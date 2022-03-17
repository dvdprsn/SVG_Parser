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
    let divDis = document.getElementById("viewer");

    if(filename == 'NULL') {
        dataHtml = '';
        tableBody.innerHTML = dataHtml;
        divDis.style.display = "none";
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

            divDis.style.display = "block";

            const dropOptions = document.getElementById('attrSel');
            let options = '';
            options += `<option value="NULL"> Select </option>`;
            dropOptions.innerHTML = options;

            const attrTable = document.getElementById('attrTable');
            let attrHtml = '';
            attrTable.innerHTML = attrHtml;
            fillAttrSel(filename);

        },
        fail: function(error) {
            console.error(error);
        }

    });

    

}
function circAttrs(idx) {
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    var filename = document.getElementById('svgFileSel').value;
    console.log(filename);

    // Fill Attribute table

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointCircAttr',
        data: {
            file: filename,
            index: idx
        },
        success: function (data) {

            let attrs = data.attrs;
            dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

            for(let i = 0; i < attrs.length; i++) {
                let attr = attrs[i];
                dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
            }

            tableBody.innerHTML = dataHtml;
        },
        fail: function (error) {
            console.error(error);
        }
    });
}
function rectAttrs(idx) {
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    var filename = document.getElementById('svgFileSel').value;
    console.log(filename);

    // Fill Attribute table

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointRectAttr',
        data: {
            file: filename,
            index: idx
        },
        success: function (data) {

            let attrs = data.attrs;
            dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

            for(let i = 0; i < attrs.length; i++) {
                let attr = attrs[i];
                dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
            }

            tableBody.innerHTML = dataHtml;
        },
        fail: function (error) {
            console.error(error);
        }
    });
}
function pathAttrs(idx) {
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    var filename = document.getElementById('svgFileSel').value;
    console.log(filename);

    // Fill Attribute table

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointPathAttr',
        data: {
            file: filename,
            index: idx
        },
        success: function (data) {

            let attrs = data.attrs;
            dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

            for(let i = 0; i < attrs.length; i++) {
                let attr = attrs[i];
                dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
            }

            tableBody.innerHTML = dataHtml;
        },
        fail: function (error) {
            console.error(error);
        }
    });
}

function groupAttrs(idx) {
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    var filename = document.getElementById('svgFileSel').value;
    console.log(filename);

    // Fill Attribute table

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointGroupAttr',
        data: {
            file: filename,
            index: idx
        },
        success: function (data) {

            let attrs = data.attrs;
            dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

            for(let i = 0; i < attrs.length; i++) {
                let attr = attrs[i];
                dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
            }

            tableBody.innerHTML = dataHtml;
        },
        fail: function (error) {
            console.error(error);
        }
    });
}

function svgAttrs() {
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    var filename = document.getElementById('svgFileSel').value;
    console.log("selected svg");
   
    // Fill Attribute table

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointSVGAttr',
        data: {
            file: filename
        },
        success: function (data) {
            let name = data.title;
            let desc = data.desc;
            let attrs = data.attrs;
            dataHtml += `<tr class="rowHeader">
            <td>Title</td> <td>Description</td>
            </tr>`;
            dataHtml += `<tr>
            <td>${name}</td> <td>${desc}</td>
            </tr>`;
            dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

            for(let i = 0; i < attrs.length; i++) {
                let attr = attrs[i];
                dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
            }

            tableBody.innerHTML = dataHtml;
        },
        fail: function (error) {
            console.error(error);
        }
    });

    
}

var attrDrop = document.getElementById('attrSel');
attrDrop.onchange = function () {
    var select = document.getElementById('attrSel').value;
    const tableBody = document.getElementById('attrTable');
    let dataHtml = '';
    console.log(select);
    tableBody.innerHTML = dataHtml;
    if(select == "svg") {
        svgAttrs();
    } else if (select.includes("circ")) {
        let tmp = select.split('.').pop();
        circAttrs(tmp);
    } else if (select.includes("rect")) {
        let tmp = select.split('.').pop();
        rectAttrs(tmp);
    }else if (select.includes("path")) {
        let tmp = select.split('.').pop();
        pathAttrs(tmp);
    }else if (select.includes("group")) {
        let tmp = select.split('.').pop();
        groupAttrs(tmp);
    }
    
}

function fillAttrSel(filename) {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointViewer',
        data: {
            file: filename
        },
        success: function (data) {
            let paths = data.path;
            let rects = data.rect;
            let circs = data.circ;
            let groups = data.group;


            const dropOptions = document.getElementById('attrSel');
            
            let options = '';
            options += `<option value="svg"> SVG </option>`;
            for (let i = 0; i < rects.length; i++) {
                let elem = rects[i];

                console.log(elem);
                options += `<option value="rect.${i}"> Rect ${i+1} </option>`;
            }
            for (let i = 0; i < circs.length; i++) {
                let elem = circs[i];

                console.log(elem);
                options += `<option value="circ.${i}"> Circle ${i+1} </option>`;
            }
            for (let i = 0; i < paths.length; i++) {
                let elem = paths[i];

                console.log(elem);
                options += `<option value="path.${i}"> Path ${i+1} </option>`;
            }
            
            for (let i = 0; i < groups.length; i++) {
                let elem = groups[i];

                console.log(elem);
                options += `<option value="group.${i}"> Group ${i+1} </option>`;
            }
            dropOptions.innerHTML += options;
            console.log("Contents of DropDown Loaded Successfully");

        },
        fail: function (error) {
            console.error(error);
        }
    });
}