if (window.File && window.FileReader && window.FileList && window.Blob) {
  // Great success! All the File APIs are supported.
} else {
  alert('The File APIs are not fully supported in this browser.');
}

window.encodedData = null;

function handleFileSelect(evt) {
  var reader = new FileReader();
  reader.onload = function (e) {
        // window.encodedData = e.target.result;
        console.log("result", e.target.result)
    };
  
  reader.readAsBinaryString(evt.target.files[0]);
}

document.getElementById('fileSelect')
  .addEventListener('change', handleFileSelect, false);