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
            console.log("Filled SVG Log Successfully");

        },
        fail: function (error) {
            console.error(error);
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
            console.log("Contents of DropDown Loaded Successfully");

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

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointViewer',
        data: {
            file: filename
        },
        success: function(data) {
            let title = data.title;
            let desc = data.desc;
            let paths = data.path;
            let rects = data.rect;
            let circs = data.circ;
            let groups = data.group;

            dataHtml += `<tr>
            <td colspan = "5"><img width="800" src="uploads/${filename}"/></td> 
            </tr>`;

            dataHtml += `<tr class="rowHeader">
            <td>Title</td> <td colspan= "3">Description</td>
            </tr>`;

            dataHtml += `<tr>
            <td>${title} </td>
            <td colspan = "2">${desc} </td>
            </tr>`;

            dataHtml += `<tr class="rowHeader">
            <td>Component</td>
            <td>Summary</td>
            <td>Other Attributes</td>
            </tr>`;

            for(let i = 0; i < rects.length; i++) {
                let rect = rects[i];
                dataHtml += `<tr>
                <td>Rectangle ${i+1}</td>
                <td>Upper Left Corner: x = ${rect.x}${rect.units}, y = ${rect.y}${rect.units} <br>
                    Width: ${rect.w}${rect.units}, Height = ${rect.h}${rect.units}
                </td>
                <td>${rect.numAttr}</td>
                </tr>`;
            }
            for(let i = 0; i < circs.length; i++) {
                let circ = circs[i];
                dataHtml += `<tr>
                <td>Circle ${i+1}</td>
                <td>Centre: x = ${circ.cx}${circ.units}, y = ${circ.cy}${circ.units},
                    radius: ${circ.r}${circ.units}
                </td>
                <td>${circ.numAttr}</td>
                </tr>`;
            }

            for(let i = 0; i < paths.length; i++) {
                let path = paths[i];
                dataHtml += `<tr>
                <td>Path ${i+1}</td>
                <td>${path.d}</td>
                <td>${path.numAttr}</td>
                </tr>`;
            }

            for(let i = 0; i < groups.length; i++) {
                let group = groups[i];
                dataHtml += `<tr>
                <td>Group ${i+1}</td>
                <td>${group.children} Child Elements</td>
                <td>${group.numAttr}</td>
                </tr>`;
            }
            

        tableBody.innerHTML = dataHtml;
        console.log("Loaded View Successfully");
        },
        fail: function(error) {
            console.error(error);
        }

    });

    

}


