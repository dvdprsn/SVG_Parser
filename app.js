'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
    //Feel free to change the contents of style.css to prettify your Web app
    res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
    fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
        const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
        res.contentType('application/javascript');
        res.send(minimizedContents._obfuscatedCode);
    });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
    if (!req.files) {
        return res.status(400).send('No files were uploaded.');
    }

    let uploadFile = req.files.uploadFile;

    // Use the mv() method to place the file somewhere on your server
    uploadFile.mv('uploads/' + uploadFile.name, function (err) {
        if (err) {
            return res.status(500).send(err);
        }

        res.redirect('/');
    });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
    fs.stat('uploads/' + req.params.name, function (err, stat) {
        if (err == null) {
            res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
        } else {
            console.log('Error in file downloading route: ' + err);
            res.send('');
        }
    });
});

//******************** Your code goes here ******************** 

let sharedLib = ffi.Library('./parser/bin/libsvgparser', {
    'wrapSVGtoJSON': ['string',['string', 'string']],
    'getTitleWrap': ['string', ['string', 'string']],
    'getDescWrap': ['string', ['string', 'string']],
    'getPathWrap': ['string', ['string', 'string']],
    'getRectWrap': ['string', ['string', 'string']],
    'getCircWrap': ['string', ['string', 'string']],
    'getGroupWrap': ['string', ['string', 'string']]
});

app.get('/endpointFilesize', function (req, res) {
    let retStr = getFilesize(req.query.filename) + "KB";
    res.send(
        {
            fileSize: retStr,
            name: req.query.filename
        }
    );
});

app.get('/endpointDir', function (req, res) {
    let files = getFilesList(req.query.dir);
    let sizes = getSizesList(files);
    let data = getDataLists(files);

    res.send(
        {
            fileArr: files,
            sizeArr: sizes,
            dataArr: data
        }
    );
});

app.get('/endpointFiles', function (req, res) {
    let files = getFilesList(req.query.dir);
    res.send(
        {
            fileArr: files
        }
    );
});

app.get('/endpointViewer', function(req,res) {
    let tmp;
    let file = req.query.file;
    let path = "uploads/" + file;
    let validation = "svg.xsd";
    let name = sharedLib.getTitleWrap(path, validation);
    let descrip = sharedLib.getDescWrap(path, validation);

    tmp = sharedLib.getPathWrap(path,validation);
    let paths = JSON.parse(tmp);

    tmp = sharedLib.getRectWrap(path, validation)
    let rects = JSON.parse(tmp);

    tmp = sharedLib.getCircWrap(path, validation);
    let circs = JSON.parse(tmp);

    tmp = sharedLib.getGroupWrap(path, validation);
    let groups = JSON.parse(tmp);

    res.send(
        {
            title: name,
            desc: descrip,
            path: paths, 
            rect: rects,
            circ: circs,
            group: groups
        }
    )
    console.log(path);
});

function getFilesList(directory) {
    // Validate files before adding to the list
    let files = fs.readdirSync(directory);
    for (let i = 0; i < files.length; i++) {
        let file = files[i];
        let tmp = file.split('.').pop();
        if (tmp != "svg") {
            files.splice(i, 1);
        }
    }
    return files;
}
function getSizesList(files) {
    let sizes = [];
    for (let i = 0; i < files.length; i++) {
        let file = files[i];
        let size = getFilesize(file) + "KB";
        sizes.push(size);
    }

    return sizes;
}

function getDataLists(files) {
    let data = [];
    for(let i = 0; i < files.length; i++) {
        let file = files[i];
        let tmp = sharedLib.wrapSVGtoJSON("uploads/"+file, "svg.xsd");
        let parsed = JSON.parse(tmp);
        data.push(parsed);
    }
    return data;
}

function getFilesize(filename) {
    try {
        var stats = fs.statSync("uploads/" + filename);
    } catch (error) {
        console.log(error);
    }

    var fileSize = stats.size / 1024;
    fileSize = Math.round(fileSize);
    return fileSize;
}


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);


