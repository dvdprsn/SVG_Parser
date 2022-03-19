// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function () {
	fillLog();
	fillDropMenu();
});

// On Window reload
window.onbeforeunload = function () {
	window.scrollTo(0, 0);
	document.getElementById("scaleSel").selectedIndex = 0;
};
// For Scaling of SVG
document.getElementById("scale").onclick = () => {
	let elemSel = document.getElementById("scaleSel").value;
	let scaling = document.getElementById("scaleFac").value;
	scaling = parseFloat(scaling);
	let path = "uploads/" + document.getElementById("svgFileSel").value;

	document.getElementById("scaleSel").selectedIndex = 0;
	document.getElementById("scaleFac").value = "";

	if (elemSel == "NULL") {
		console.log("Did not select an element to scale");
		alert("Did not select element type to scale");
		return;
	}
	if (scaling < 0 || scaling == "") {
		console.log("Scale cannot be less than 0!");
		alert("Cannot scale by value less than 0");
		return;
	}

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointScale",
		data: {
			scale: scaling,
			elem: elemSel,
			file: path,
		},
		success: (data) => {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to scale objects");
				alert("Failed to scale");
			} else {
				window.location.reload();
				console.log("Scaling Success!");
			}
		},
		fail: (error) => {
			alert(error);
			console.log(error);
		},
	});
};
// Adding a new attribute
document.getElementById("nAttrSub").onclick = () => {
	let nameAttr = document.getElementById("newAttrName").value.toLowerCase();
	let valueAttr = document.getElementById("newAttrVal").value.toLowerCase();
	let path = "uploads/" + document.getElementById("svgFileSel").value;
	let elemSelect = document.getElementById("attrSel").value;
	let index = elemSelect.split(".");
	if (elemSelect === "svg") {
		index.push(0);
	}
	document.getElementById("newAttrName").value = "";
	document.getElementById("newAttrVal").value = "";

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointNAttr",
		data: {
			name: nameAttr,
			value: valueAttr,
			data: index,
			file: path,
		},
		success: (data) => {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to add attribute");
				alert("failed to add attribute");
			} else {
				window.location.reload();
				console.log("New Attribute Success!!");
			}
		},
		fail: (error) => {
			console.log(error);
		},
	});
};
// Function to fill the file log
function fillLog() {
	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointDir",
		data: {
			dir: "uploads/",
		},
		success: function (data) {
			let files = data.fileArr;
			let sizes = data.sizeArr;
			let datas = data.dataArr;

			const tableBody = document.getElementById("tableLog");
			let dataHtml = "";

			if (files.length == 0) {
				// No files on server
				dataHtml += `<tr><td id="noFiles" colspan = "7">No files</td></tr>`;
			} else {
				for (let i = 0; i < files.length; i++) {
					//For each file found on server
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
			console.log(error);
		},
	});
}
// Populates the dropdown menu of files
function fillDropMenu() {
	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointFiles",
		data: {
			dir: "uploads/",
		},
		success: function (data) {
			let files = data.fileArr;

			const dropOptions = document.getElementById("svgFileSel");

			let options = "";

			for (let i = 0; i < files.length; i++) {
				let file = files[i];

				options += `<option value="${file}"> ${file} </option>`;
			}

			dropOptions.innerHTML += options;
			console.log("Contents of file select Loaded Successfully");
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// Once an SVG file is selected
var dropTest = document.getElementById("svgFileSel");
dropTest.onchange = () => {
	var filename = document.getElementById("svgFileSel").value;
	const tableBody = document.getElementById("viewPanel");
	let dataHtml = "";
	let divDis = document.getElementById("viewer");

	var editDiv = document.getElementById("editAttr");
	editDiv.style.display = "none";

	if (filename == "NULL") {
		dataHtml = "";
		tableBody.innerHTML = dataHtml;
		divDis.style.display = "none";
		return;
	}
	console.log("Selected -> " + filename);

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointViewer",
		data: {
			file: filename,
		},
		success: function (data) {
			let title = data.title;
			let desc = data.desc;

			let paths = JSON.parse(data.path);
			let rects = JSON.parse(data.rect);
			let circs = JSON.parse(data.circ);
			let groups = JSON.parse(data.group);

			dataHtml += `<tr>
            <td colspan = "5"><img width="800" src="uploads/${filename}"/></td> 
            </tr>`;

			dataHtml += `<tr class="rowHeader">
            <td>Title</td> <td colspan= "3">Description</td>
            </tr>`;

			dataHtml += `<tr>
            <td>${title}</td>
            <td colspan = "2">${desc}</td>
            </tr>`;

			dataHtml += `<tr class="rowHeader">
            <td>Component</td>
            <td>Summary</td>
            <td>Other Attributes</td>
            </tr>`;

			for (let i = 0; i < rects.length; i++) {
				let rect = rects[i];
				dataHtml += `<tr>
                <td>Rectangle ${i + 1}</td>
                <td>Upper Left Corner: x = ${rect.x}${rect.units}, y = ${rect.y}${rect.units} <br>
                    Width: ${rect.w}${rect.units}, Height = ${rect.h}${rect.units}
                </td>
                <td>${rect.numAttr}</td>
                </tr>`;
			}
			for (let i = 0; i < circs.length; i++) {
				let circ = circs[i];
				dataHtml += `<tr>
                <td>Circle ${i + 1}</td>
                <td>Centre: x = ${circ.cx}${circ.units}, y = ${circ.cy}${circ.units},
                    radius: ${circ.r}${circ.units}
                </td>
                <td>${circ.numAttr}</td>
                </tr>`;
			}

			for (let i = 0; i < paths.length; i++) {
				let path = paths[i];
				dataHtml += `<tr>
                <td>Path ${i + 1}</td>
                <td>path data = ${path.d}</td>
                <td>${path.numAttr}</td>
                </tr>`;
			}

			for (let i = 0; i < groups.length; i++) {
				let group = groups[i];
				dataHtml += `<tr>
                <td>Group ${i + 1}</td>
                <td>${group.children} Child Elements</td>
                <td>${group.numAttr}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
			console.log("Loaded View Successfully");

			divDis.style.display = "block";

			const dropOptions = document.getElementById("attrSel");
			let options = "";
			options += `<option value="NULL"> Select </option>`;
			dropOptions.innerHTML = options;

			const attrTable = document.getElementById("attrTable");
			let attrHtml = "";
			attrTable.innerHTML = attrHtml;
			fillAttrSel(filename);

			changeTitleDesc(title, desc);
		},
		fail: function (error) {
			console.log(error);
		},
	});
};

// For changing the title or description of the SVG
var titleSub = document.getElementById("tdSub");
titleSub.onclick = function () {
	let titleField = document.getElementById("title");
	let descField = document.getElementById("desc");
	let file = document.getElementById("svgFileSel").value;
	if (titleField.value.length > 256 || descField.value.length > 256) {
		console.log("Title or Desc is too long!");
		alert("Title or Desc is too long");
		return;
	}
	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointTC",
		data: {
			name: titleField.value,
			desc: descField.value,
			filename: file,
		},
		success: function (data) {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to change title or desc");
				alert("Failed to change title or desc");
			} else {
				window.location.reload();
				console.log("Change Success!!");
			}
		},
		fail: function (error) {
			console.log(error);
		},
	});
};
//Adding a new rectanlge
document.getElementById("addRect").onclick = () => {
	let fields = document.getElementsByClassName("addRect");
	let values = [];
	let file = document.getElementById("svgFileSel").value;

	for (let i = 0; i < fields.length; i++) {
		if (fields[i].value == "" && i != fields.length - 1) {
			console.log("Required items cannot be empty");
			alert("Required items cannot be empty");
			return;
		}
		values.push(fields[i].value);
		fields[i].value = "";
	}
	let jsonString = JSON.stringify({
		x: parseFloat(values[0]),
		y: parseFloat(values[1]),
		w: parseFloat(values[2]),
		h: parseFloat(values[3]),
		units: values[4],
	});

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointAddRect",
		data: {
			fieldData: jsonString,
			filename: file,
		},
		success: (data) => {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to Add Rect");
				alert("failed to add rect");
			} else {
				window.location.reload();
				console.log("Added new rect!");
			}
		},
		fail: (error) => {
			console.log(error);
		},
	});
};
// Adding a new circle
document.getElementById("addCirc").onclick = () => {
	console.log("Adding Init Circle");
	let fields = document.getElementsByClassName("addCircle");
	let values = [];
	let file = document.getElementById("svgFileSel").value;

	for (let i = 0; i < fields.length; i++) {
		if (fields[i].value == "" && i != fields.length - 1) {
			console.log("Required items cannot be empty");
			alert("Required items cannot be empty");
			return;
		}
		values.push(fields[i].value);
		fields[i].value = "";
	}

	let jsonString = JSON.stringify({
		cx: parseFloat(values[0]),
		cy: parseFloat(values[1]),
		r: parseFloat(values[2]),
		units: values[3],
	});

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointAddCirc",
		data: {
			fieldData: jsonString,
			filename: file,
		},
		success: (data) => {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to Add Circle");
				alert("Failed to add circle");
			} else {
				window.location.reload();
				console.log("Added new circle!");
			}
		},
		fail: (error) => {
			console.log(error);
		},
	});
};

// Create a new empty SVG
var newSVGSub = document.getElementById("nSVGSub");
newSVGSub.onclick = () => {
	let titleField = document.getElementById("newTitle");
	let title = titleField.value;
	titleField.value = "";
	let descField = document.getElementById("newDesc");
	let desc = descField.value;
	descField.value = "";
	let fileField = document.getElementById("newFN");
	let file = fileField.value;
	fileField.value = "";
	if (file === "") {
		console.log("File name cannot be empty");
		alert("filename cannot be emtpy");
		return;
	}
	if (title > 256 || desc > 256 || file > 256) {
		console.log("Title or Desc is too long!");
		alert("An element exceeds the limit (256 char)");
		return;
	}
	if (file.includes(".")) {
		console.log("Invalid file name");
		alert("Filename should not contain a '.' char");
		return;
	}
	if (file.includes("/")) {
		console.log("Please do not include path!");
		alert("Filename should not contain path information");
		return;
	}
	let jsonArr = { title: title, descr: desc };

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointNSVG",
		data: {
			jsonData: jsonArr,
			filename: file,
		},
		success: function (data) {
			let valid = data.succ;
			if (valid == "f") {
				console.log("Failed to create new SVG");
				alert("failed to create new SVG");
			} else {
				window.location.reload();
				console.log("Empty SVG Creation Success!");
			}
		},
		fail: function (error) {
			console.log(error);
		},
	});
};
// Sets the input field to the arguments
function changeTitleDesc(title, desc) {
	const titleField = document.getElementById("title");
	const descField = document.getElementById("desc");
	titleField.value = title;
	descField.value = desc;
}
// Gets the circles attributes and populate table
function circAttrs(idx) {
	const tableBody = document.getElementById("attrTable");
	let dataHtml = "";
	var filename = document.getElementById("svgFileSel").value;

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointCircAttr",
		data: {
			file: filename,
			index: idx,
		},
		success: function (data) {
			let attrs = data.attrs;
			dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

			for (let i = 0; i < attrs.length; i++) {
				let attr = attrs[i];
				dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// Get rectangles attributes and populate table
function rectAttrs(idx) {
	const tableBody = document.getElementById("attrTable");
	let dataHtml = "";
	var filename = document.getElementById("svgFileSel").value;
	// console.log(filename);

	// Fill Attribute table

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointRectAttr",
		data: {
			file: filename,
			index: idx,
		},
		success: function (data) {
			let attrs = data.attrs;
			dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

			for (let i = 0; i < attrs.length; i++) {
				let attr = attrs[i];
				dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// Get paths attributes and populate table
function pathAttrs(idx) {
	const tableBody = document.getElementById("attrTable");
	let dataHtml = "";
	var filename = document.getElementById("svgFileSel").value;
	// console.log(filename);

	// Fill Attribute table

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointPathAttr",
		data: {
			file: filename,
			index: idx,
		},
		success: function (data) {
			let attrs = data.attrs;
			dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

			for (let i = 0; i < attrs.length; i++) {
				let attr = attrs[i];
				dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// Get attributes for groups based on index
function groupAttrs(idx) {
	const tableBody = document.getElementById("attrTable");
	let dataHtml = "";
	var filename = document.getElementById("svgFileSel").value;

	// Fill Attribute table

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointGroupAttr",
		data: {
			file: filename,
			index: idx,
		},
		success: function (data) {
			let attrs = data.attrs;
			dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

			for (let i = 0; i < attrs.length; i++) {
				let attr = attrs[i];
				dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// Get other attributes of the SVG struct itself
function svgAttrs() {
	const tableBody = document.getElementById("attrTable");
	let dataHtml = "";
	var filename = document.getElementById("svgFileSel").value;
	// console.log("selected svg");

	// Fill Attribute table

	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointSVGAttr",
		data: {
			file: filename,
		},
		success: function (data) {
			let attrs = data.attrs;

			dataHtml += `<tr class="rowHeader">
            <td colspan="2">Attributes</td> 
            </tr>`;

			for (let i = 0; i < attrs.length; i++) {
				let attr = attrs[i];
				dataHtml += `<tr>
                <td>${attr.name}</td> <td>${attr.value}</td>
                </tr>`;
			}

			tableBody.innerHTML = dataHtml;
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
// On the selection of an attribute from the drop down
var attrDrop = document.getElementById("attrSel");
attrDrop.onchange = function () {
	var select = document.getElementById("attrSel").value;
	const tableBody = document.getElementById("attrTable");
	var editDiv = document.getElementById("editAttr");
	let dataHtml = "";
	if (select == "NULL") {
		editDiv.style.display = "none";
	} else {
		editDiv.style.display = "block";
	}

	// console.log(select);
	tableBody.innerHTML = dataHtml;
	if (select == "svg") {
		svgAttrs();
	} else if (select.includes("circ")) {
		// These splits grab the index
		let tmp = select.split(".").pop();
		circAttrs(tmp);
	} else if (select.includes("rect")) {
		let tmp = select.split(".").pop();
		rectAttrs(tmp);
	} else if (select.includes("path")) {
		let tmp = select.split(".").pop();
		pathAttrs(tmp);
	} else if (select.includes("group")) {
		let tmp = select.split(".").pop();
		groupAttrs(tmp);
	}
};
// Fills the drop down menu containing components
function fillAttrSel(filename) {
	$.ajax({
		type: "get",
		dataType: "json",
		url: "/endpointViewer",
		data: {
			file: filename,
		},
		success: function (data) {
			let paths = data.path;
			let rects = data.rect;
			let circs = data.circ;
			let groups = data.group;

			const dropOptions = document.getElementById("attrSel");

			let options = "";
			options += `<option value="svg"> SVG </option>`;
			for (let i = 0; i < rects.length; i++) {
				let elem = rects[i];

				// console.log(elem);
				options += `<option value="rect.${i}"> Rect ${i + 1} </option>`;
			}
			for (let i = 0; i < circs.length; i++) {
				let elem = circs[i];

				// console.log(elem);
				options += `<option value="circ.${i}"> Circle ${i + 1} </option>`;
			}
			for (let i = 0; i < paths.length; i++) {
				let elem = paths[i];

				// console.log(elem);
				options += `<option value="path.${i}"> Path ${i + 1} </option>`;
			}

			for (let i = 0; i < groups.length; i++) {
				let elem = groups[i];

				// console.log(elem);
				options += `<option value="group.${i}"> Group ${i + 1} </option>`;
			}
			dropOptions.innerHTML += options;
			console.log("Contents of DropDown Loaded Successfully");
		},
		fail: function (error) {
			console.log(error);
		},
	});
}
