var expect = require('chai').expect;
var Emsc = require('../out/dctuncomp.js');
var fs = require('fs');
var path = require('path');


var EmscEnv = {};


// Create example data to test float_multiply_array
// var data = new Float32Array([1, 2, 3, 4, 5]);

// Get data byte size, allocate memory on Emscripten heap, and get pointer
// var nDataBytes = data.length * data.BYTES_PER_ELEMENT;
// var dataPtr = Emsc._malloc(nDataBytes);

// Copy data to Emscripten heap (directly accessed from Module.HEAPU8)
// var dataHeap = new Uint8Array(Emsc.HEAPU8.buffer, dataPtr, nDataBytes);
// dataHeap.set(new Uint8Array(data.buffer));

// Call function and get result
// float_multiply_array(2, dataHeap.byteOffset, data.length);
// var result = new Float32Array(dataHeap.buffer, dataHeap.byteOffset, data.length);

// // Free memory
// Module._free(dataHeap.byteOffset);

describe("DCT Decompress", function() {	
	
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
			decompress = Emsc.cwrap('decompress', 'number', ['number','number']);
			outputBuffer = new ArrayBuffer(imageSizeInBytes);
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
		
		it('can decompress', function() {
			function alloc(buffer) {
				return new Uint8Array(Emsc.HEAPU8.buffer, Emsc._malloc(buffer.byteLength), buffer.byteLength);
			}
			
			function allocAndCopy(buffer) {
				var dataHeap = alloc(buffer);
				dataHeap.set(new Uint8Array(buffer));	
				return dataHeap;	
			}
			
			var headerLength = inputBuffer.byteLength - 475858;
			console.log("# Bytes in Header", headerLength)

			var headerInts = new Uint32Array(inputBuffer.slice(0, 20));
			var headerFloats = new Float32Array(inputBuffer.slice(20,28));			
			
			
			console.log("Header Ints", headerInts)
			console.log("Header Floats", headerFloats)
			
			var input = allocAndCopy(inputBuffer);
			var output = alloc(outputBuffer);
			
			console.log("bbbs")
			console.log("Input Buffer", input.subarray(0, 50))
			
			var rt = decompress(input.byteOffset, output.byteOffset);
			
			var min = 999999999999, max = -1;
			console.log("return code", rt)
			for (var i = 0; i < 10; i++) {
				console.log("JS OUT", Emsc.getValue(output.byteOffset + i, 'float'));
			}
			console.log("Chars min", min)
			console.log("Chars max", max)
			
			var outputFloats = new Float32Array(Emsc.HEAPU8.buffer, output.byteOffset, outputBuffer.length);
			min = 999999999999, max = -1;
			for (var i = 0; i < 10; i++) {
				console.log("JS FLT", outputFloats[i])
			}
			console.log("Floats min", min)
			console.log("Floats max", max)
			
			Emsc._free(input.byteOffset);
			Emsc._free(output.byteOffset);
		})
		
	});
	
	
	// it('should do what...', function (done) {
		
	// });
	
	
	
	
})