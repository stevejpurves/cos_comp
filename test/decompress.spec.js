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
		
		var decompress = null;
		
		before(function (done) {
			decompress = Emsc.cwrap('decompress', 'number', ['number','number']);
			outputBuffer = new ArrayBuffer(SIZE_OF_FLOAT*n1*n2);
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
			function allocAndCopy(buffer) {
				var dataHeap = new Uint8Array(Emsc.HEAPU8.buffer, Emsc._malloc(buffer.byteLength), buffer.byteLength);
				dataHeap.set(new Uint8Array(buffer));			
				return dataHeap;	
			}
			
			var headerLength = inputBuffer.byteLength - 475858;
			console.log("# Bytes in Header", headerLength)

			var headerInts = new Uint32Array(inputBuffer.slice(0, 20));
			var headerFloats = new Float32Array(inputBuffer.slice(20,28));			
			var results = new Float32Array(outputBuffer);
			
			console.log("Header Ints", headerInts)
			console.log("Header Floats", headerFloats)
			
			var input = allocAndCopy(inputBuffer);
			var output = allocAndCopy(outputBuffer);
			
			console.log("Floats before", results.subarray(0, 20))
			
			var rt = decompress(input.byteOffset, output.byteOffset);
			
			console.log("return code", rt)
			console.log("Floats after", results.subarray(0,20))
		})
		
	});
	
	
	// it('should do what...', function (done) {
		
	// });
	
	
	
	
})