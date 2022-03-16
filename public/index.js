// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function () {

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function (e) {
        $('#blah').html("Form has data: " + $('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
            type: 'get',
            dataType: 'json',
            url: '/endpointFilesize',
            data: {
                filename: $('#entryBox').val()
            },
            success: function (data) {
                $('#size').html(data.fileSize);
                $('#entryBox').val(''); // Clear the box
                // console.log(data);
            },
            fail: function (error) {
                console.log(error);
            }
        });
    });

    fillLog();
    fillDropMenu();


});

function fillLog() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointDir',
        data: {
            dir: "uploads/"
        },
        success: function (data) {
            console.log("fillLog AJAX Success");

            let files = data.fileArr;
            let sizes = data.sizeArr;
            let datas = data.dataArr;

            const tableBody = document.getElementById('tableLog');
            let dataHtml = '';

            if (files.length == 0) { // No files on server
                dataHtml += `<tr><td id="noFiles" colspan = "7">No files</td></tr>`;
            } else {
                for (let i = 0; i < files.length; i++) { //For each file found on server
                    console.log("Adding file to table");
                    let filename = files[i];
                    let size = sizes[i];
                    let data = datas[i];
                    dataHtml += `<tr>
                    <td><a href="${filename}" download><img width="200" src="uploads/${filename}"/></a></td> 
                    <td><a href="${filename}" download>${filename}</a> </td>
                    <td>${size}</td>
                    <td>${data.numRect}</td> 
                    <td>${data.numCirc}</td>
                    <td>${data.numPaths}</td>
                    <td>${data.numGroups}</td>

                    </tr>`;
                }
            }

            tableBody.innerHTML = dataHtml;

        },
        fail: function (error) {
            console.log(error);
        }
    });
}

function fillDropMenu() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointFiles',
        data: {
            dir: "uploads/"
        },
        success: function (data) {
            let files = data.fileArr;

            const dropOptions = document.getElementById('svgFileSel');

            let options = '';

            for (let i = 0; i < files.length; i++) {
                let file = files[i];

                options += `<option value="${file}"> ${file} </option>`;
            }

            dropOptions.innerHTML += options;

        },
        fail: function (error) {
            console.error(error);
        }
    });
}

var dropTest = document.getElementById('svgFileSel');
dropTest.onchange = function () {
    var filename = document.getElementById('svgFileSel').value;
    const tableBody = document.getElementById('viewPanel');
    let dataHtml = '';

    if(filename == 'NULL') {
        dataHtml = '';
        tableBody.innerHTML = dataHtml;
        return;
    }
    console.log("Selected -> " + filename);

    

    dataHtml += `<tr>
        <td colspan = "5"><a href="${filename}" download><img width="800" src="uploads/${filename}"/></a></td> 
        </tr>`;

    dataHtml += `<tr>
        <td>Title</td> <td>Description</td>
        </tr>`;

    tableBody.innerHTML = dataHtml;
    // $.ajax({
    //     type: 'get',
    //     dataType: 'json',
    //     url: '/endpointViewer',
    //     data: {
    //         file: filename
    //     },
    // });
}


