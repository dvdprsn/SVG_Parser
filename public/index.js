// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
            type: 'get',
            dataType: 'json',
            url: '/endpointFilesize',
            data:{
                filename: $('#entryBox').val()
            }, 
            success: function(data) {
                $('#size').html(data.fileSize);
                $('#entryBox').val(''); // Clear the box
                // console.log(data);
            },
            fail: function(error) {
                console.log(error);
            }
        });
    });

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/endpointDir',
        data:{
            dir: "uploads/"
        },
        success: function(data) {
            console.log(data.fileArr);
            let files = data.fileArr;
            let sizes = data.sizeArr;

            const tableBody = document.getElementById('tableLog');
            let dataHtml = '';
            if(files.length == 0) {
                dataHtml += `<tr><td id="noFiles" colspan = "7">No files</td></tr>`;
            } else {
                for(let i = 0; i < files.length; i++) {
                    console.log("Here -> " + files[i]);
                    let filename = files[i];
                    let size = sizes[i];
                    dataHtml += `<tr><td><a href="${filename}" download><img width="200" src="uploads/${filename}"/></a></td> <td><a href="${filename}" download>${filename}</a> </td><td>${size}</td></tr>`;
                }
            }

            tableBody.innerHTML = dataHtml;
            
        }, 
        fail: function(error) {
            console.log(error);
        }
    });

});


