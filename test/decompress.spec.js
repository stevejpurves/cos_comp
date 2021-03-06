var expect = require('chai').expect;
var Emsc = require('../out/dctuncomp.js');
var fs = require('fs');
var path = require('path');
var BinaryFile =  require('binary-file')


var DCTComp = (function(emsc) {
	
	var decompFn = Emsc.cwrap('decompress', 'number', ['number','number']);
	
	return {
		Emsc: emsc,
		alloc: function(length) {
			return new Uint8Array(Emsc.HEAPU8.buffer, Emsc._malloc(length), length);
		},
		allocAndCopy: function allocAndCopy(buffer) {
			var newBufferOnHeap = this.alloc(buffer.byteLength);
			newBufferOnHeap.set(new Uint8Array(buffer));	
			return newBufferOnHeap;
		},
		decompress: decompFn
	};
	
})(Emsc);

describe("DCT Decompress", function() {	
	this.timeout(20000)
	
	it("decompress function should be visible", function() {
		expect(Emsc._decompress).to.not.be.undefined;
		expect(Emsc.cwrap).to.not.be.undefined;
		expect(typeof Emsc._decompress).to.equal('function');
	});
	
	describe('from file', function () {
		var filename = path.join(__dirname, '../data/strip.bin.clip.dct');
		var inputBuffer, outputBuffer;
		var n1 = 751, n2 = 1200;
		var SIZE_OF_FLOAT = 4;
		var imageSizeInBytes = n1*n2*SIZE_OF_FLOAT;
		
		var decompress = null;
		
		before(function (done) {
			// decompress = Emsc.cwrap('decompress', 'number', ['number','number']);
			fs.readFile(filename, function(err, buffer) {
				if (err) return done(err);
				console.log("length of buffer", buffer.length)
				inputBuffer = new ArrayBuffer(buffer.length)
				var chars = new Uint8Array(inputBuffer);
				for (var i = 0; i < buffer.length; i++) {
					chars[i] = buffer[i];
				}
				done();
			})
		});
		
		it('can decompress', function(done) {
			var headerLength = inputBuffer.byteLength - 475858;
			console.log("# Bytes in Header", headerLength)

			var headerInts = new Uint32Array(inputBuffer.slice(0, 20));
			var headerFloats = new Float32Array(inputBuffer.slice(20,28));			
						
			console.log("Header Ints", headerInts)
			console.log("Header Floats", headerFloats)
			
			
			var begin = process.hrtime();
			var input = DCTComp.allocAndCopy(inputBuffer);
			var output = DCTComp.alloc(imageSizeInBytes);
						
			var start = process.hrtime();
			var rt = DCTComp.decompress(input.byteOffset, output.byteOffset);
			var elapsed = process.hrtime(start);
			var totalElapsed = process.hrtime(begin);
			
			
			console.log("first 10 floats")
			var outputFloats = new Float32Array(Emsc.HEAPU8.buffer, output.byteOffset, output.byteLength);		
			var outputUchar = new Uint8Array(Emsc.HEAPU8.buffer, output.byteOffset, imageSizeInBytes);	
			for (var i = 0; i < 10; i++) {
				console.log("JS FLT", outputFloats[i])
			}
			for (var i = 0; i < 10; i++) {
				console.log("OUT", outputUchar[i])
			}
			console.log("output.byteLength", output.byteOffset, output.byteLength);
			console.log("Decompression took", elapsed[1]/1e6, "ms")
			console.log("Decompression + HEAP Setup took", totalElapsed[1]/1e6, "ms")

			var wstream = fs.createWriteStream('output.bin');
			wstream.write(new Buffer(outputUchar));
			wstream.end()

			wstream.on("finish", function() {
				done();
			})
		})
	});
	
})