#include "SVGHelpers.h"

void parser(xmlNode *a_node, SVG *svg) {

    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {

       char *nodeName = "\0";

        if (cur_node->type == XML_ELEMENT_NODE) {

            nodeName = (char *)cur_node->name;

            if(strcmp(nodeName, "title") == 0) { //Works
                setSVGTitle(svg, cur_node->children);
            } else if (strcmp(nodeName, "desc") == 0) { //Works
                setSVGDesc(svg, cur_node->children);
            } else if (strcmp(nodeName, "svg") == 0) { //Works
                fillSVG(svg, cur_node);
            } else if (strcmp(nodeName, "path") == 0) { 
                insertBack(svg->paths,createPath(cur_node));
            }else if (strcmp(nodeName, "rect") == 0) { //WORKS
                insertBack(svg->rectangles,createRect(cur_node));
            } else if (strcmp(nodeName, "circle") == 0) { //WORKS
                insertBack(svg->circles,createCircle(cur_node));
            } else if(strcmp(nodeName, "g") == 0) {
                Group *g; 
                g = malloc(sizeof(Group)+30);
                if(g == NULL) return;
                initGroup(g);
                createGroup(g,cur_node);
                insertBack(svg->groups,g);
                cur_node = cur_node->next; //Forwards node so children of group are not added to other lists
            }

        }

        parser(cur_node->children, svg);

    }

}

Rectangle *createRect(xmlNode *cur_node) {

    Rectangle *rect = malloc(sizeof(Rectangle)+30);
    initRect(rect);
    char *ptr;
    xmlAttr *attr;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;

        char *attrName = (char *) attr->name;
        char *cont = (char *) (value->content);

        if (strcmp(attrName, "x") == 0) {
            rect->x = atof(cont);

            strtoul(cont, &ptr, 10); // gets decimal onwards 1.214cm -> .214cm
            if(ptr[0] == '.') ptr += 1; //Shifts pointer past decimal .214cm -> 214cm
            strtoul(ptr, &ptr, 10); //removes numbers from char 214cm -> cm
            strcpy(rect->units,ptr);

        } else if (strcmp(attrName, "y") == 0) {
            rect->y = atof(cont);
        } else if (strcmp(attrName, "width") == 0) {
            rect->width = atof(cont);
        } else if (strcmp(attrName, "height") == 0) {
            rect->height = atof(cont);
        } else { //Other Attributes
            insertBack(rect->otherAttributes, createAttr(attrName, cont));
        }

    }

    return rect;

}

Circle *createCircle(xmlNode *cur_node) {

    Circle *circle = malloc(sizeof(Circle)+20);
    initCircle(circle);
    char *ptr;
    xmlAttr *attr;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;

        char *attrName = (char *) attr->name;
        char *cont = (char *) (value->content);

        if (strcmp(attrName, "cx") == 0) {

            circle->cx = atof(cont); // get just float 

            strtoul(cont, &ptr, 10); // gets decimal onwards 1.214cm -> .214cm
            if(ptr[0] == '.') ptr += 1; //Shifts pointer past decimal .214cm -> 214cm
            strtoul(ptr, &ptr, 10); //removes numbers from char 214cm -> cm
            strcpy(circle->units,ptr);

        } else if (strcmp(attrName, "cy") == 0) {
            circle->cy = atof(cont);
        } else if (strcmp(attrName, "r") == 0) {
            circle->r = atof(cont);
        } else { //Other Attributes
            insertBack(circle->otherAttributes, createAttr(attrName, cont));
        }

    }

    return circle;

}

void createGroup(Group *g, xmlNode *cur_node) { 

    xmlNode *a_node = cur_node->children;

    xmlAttr *attr;
    xmlNode *value;
    char *attrName;
    char *cont;
    //Fills in attributes for this group
    for(attr = cur_node->properties; attr != NULL; attr = attr->next) { //Get Attributes
            value = attr->children;

            attrName = (char *) (attr->name);
            cont = (char *) (value->content);

            insertBack(g->otherAttributes,createAttr(attrName,cont));
    
    }
    //Recursively adds structures to group lists
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) { //Go over siblings ONLY

       char *nodeName = "\0";

        if (cur_node->type == XML_ELEMENT_NODE) {

            nodeName = (char *)cur_node->name;

            if(strcmp(nodeName, "rect") == 0 ) {
                insertBack(g->rectangles, createRect(cur_node));
            } else if(strcmp(nodeName, "circle") == 0) {
                insertBack(g->circles,createCircle(cur_node));
            } else if(strcmp(nodeName,"path")==0) {
                insertBack(g->paths,createPath(cur_node));
            } else if(strcmp(nodeName, "g") == 0) { //Nested group found
                Group *groupNest; 
                groupNest = malloc(sizeof(Group)+30); //Create nested group
                initGroup(groupNest); //Init with empty lists
                insertBack(g->groups,groupNest); //insert pointer to top level group
                createGroup(groupNest,cur_node); //Fill group with children
            }

        }

    }

}

Attribute *createAttr(char *name, char value[]) {
    if(value == NULL) return NULL; //Cannot have a nsame attr without value

    Attribute *attr;
    attr = malloc(sizeof(Attribute) + sizeof(char) * (strlen(value)+30)); //Allocate for flexible array member
    if(attr == NULL) return NULL; //If alloc failed

    attr->name = malloc(sizeof(char)*strlen(name)+20); //Allocate space for the char* in attr struct
    if(attr->name == NULL) return NULL; //Check for fail
    
    strcpy(attr->name, name);
    strcpy(attr->value, value); //Copy string for value

    return attr;    //Return create attribute to caller

}

Path *createPath(xmlNode *cur_node) {
    
    xmlAttr *attr;
    xmlNode *value; 
    Path *path;

    path = malloc(sizeof(Path));
    if(path == NULL) return NULL;
    path->otherAttributes = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);


    char *attrName;
    char *cont;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {

        value = attr->children;
        attrName = (char *) (attr->name);
        cont = (char *) (value->content);


        if (strcmp(attrName, "d") == 0) {
            path = realloc(path, sizeof(path)+ (sizeof(char)*strlen(cont)+30));
            strcpy(path->data, cont);
        } else { //Other Attributes
            insertBack(path->otherAttributes, createAttr(attrName, cont));
            
        }

    }
    
    return path;

}

void initCircle(Circle *circle) {
    circle->cx = 0;
    circle->cy = 0;
    circle->r = 0;
    strcpy(circle->units, "\0");
    circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
}

void initRect(Rectangle *rect) {
    rect->height = 0;
    rect->width = 0;
    rect->x = 0;
    rect->y = 0;
    strcpy(rect->units, "");
    rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
}

void initSVG(SVG *svgReturn) {

    strcpy(svgReturn->namespace, "\0");
    strcpy(svgReturn->title, "\0");
    strcpy(svgReturn->description, "\0");

    svgReturn->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    svgReturn->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    svgReturn->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    svgReturn->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    svgReturn->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

}

void initGroup(Group *group) {
    group->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    group->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    group->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    group->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    group->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
}

void fillSVG(SVG *svg, xmlNode *cur_node) {
    xmlAttr *xmlAttr;
    xmlNode *value;
    Attribute *toAdd;
    char *attrName;
    char *cont;
    

    for (xmlAttr = cur_node->properties; xmlAttr != NULL; xmlAttr = xmlAttr->next) { //Go through all attributes on the node

        value = xmlAttr->children; 
        attrName = (char *) xmlAttr->name;
        cont = (char *) value->content;
        toAdd = createAttr(attrName, cont);
        insertBack(svg->otherAttributes, toAdd); //Insert new attr to back of attribute list for SVG

    }

}

void setSVGTitle(SVG *svg, xmlNode *cur_node) {
    
    if (cur_node->content != NULL) strcpy(svg->title, (char *) cur_node->content);

}

void setSVGDesc(SVG *svg, xmlNode *cur_node) {
    if (cur_node->content != NULL) strcpy(svg->description, (char *) cur_node->content);
}

char *getNS(xmlNode *cur_node) {
    if (cur_node) return (char *) cur_node->ns->href;
    return "";
}

void dummyDel(void *data){
    return;
}

void findRect(Group *group, List *lst) {

    //Get elements from list of rectangles in this group
    ListIterator iter = createIterator(group->rectangles);
    void *elem;
    while ((elem = nextElement(&iter))!= NULL) {
        Rectangle *rect = (Rectangle*) elem; //Cast to rectangle pointer
        insertBack(lst,rect); //Insert rectangle into toReturn list

    }
    //Iterate over groups within the list of subgroups calling findRects
    iter = createIterator(group->groups);
    while((elem = nextElement(&iter))!=NULL) {
        elem = (Group*) elem;
        findRect(elem, lst);
    }

}

void findCirc(Group *group, List *lst) {
    //Get elements from list of rectangles in this group
    ListIterator iter = createIterator(group->circles);
    void *elem;
    while ((elem = nextElement(&iter))!= NULL) {
        Circle *circ = (Circle*) elem; //Cast to rectangle pointer
        insertBack(lst,circ); //Insert rectangle into toReturn list

    }
    //Iterate over groups within the list of subgroups calling findRects
    iter = createIterator(group->groups);
    while((elem = nextElement(&iter))!=NULL) {
        Group *g = (Group*) elem;
        findCirc(g, lst);
    }
}

void findPaths(Group *group, List *lst) {
    ListIterator iter = createIterator(group->paths);
    void *elem;
    while((elem = nextElement(&iter))!=NULL) {
        Path *p = (Path *) elem;
        insertBack(lst,p);

    }
    iter = createIterator(group->groups);
    while((elem = nextElement(&iter))!=NULL) {
        Group *g = (Group *) elem;
        findPaths(g,lst);
    }
}

void findGroup(Group *group, List *lst) {
    ListIterator iter = createIterator(group->groups);
    void *elem;
    while((elem = nextElement(&iter))!= NULL) {
        Group *g = (Group *) elem;
        insertBack(lst,g);
        findGroup(g,lst);
    }
}


//-----------A2-------------

void rectToXML(xmlNodePtr pNode, Rectangle *rect) {
    xmlNodePtr rectNode = xmlNewChild(pNode, NULL, BAD_CAST "rect", NULL);
    //TODO this is a bad malloc
    char *temp = malloc((sizeof(char) * 20));

    //Get x
    sprintf(temp, "%f", rect->x);
    strcat(temp, rect->units);
    xmlNewProp(rectNode, BAD_CAST "x", BAD_CAST temp);
    

    //Get y
    sprintf(temp, "%f", rect->y);
    strcat(temp, rect->units);
    xmlNewProp(rectNode, BAD_CAST "y", BAD_CAST temp);

    //Get width
    sprintf(temp, "%f", rect->width);
    strcat(temp, rect->units);
    xmlNewProp(rectNode, BAD_CAST "width", BAD_CAST temp);

    //Get height
    sprintf(temp, "%f", rect->height);
    strcat(temp, rect->units);
    xmlNewProp(rectNode, BAD_CAST "height", BAD_CAST temp);

    //Get otherAttr
    ListIterator iter = createIterator(rect->otherAttributes);
    void *elem;
    while((elem = nextElement(&iter))!= NULL) {
        Attribute *a = (Attribute *) elem;
        xmlNewProp(rectNode, BAD_CAST a->name, BAD_CAST a->value);
    }


    free(temp);
}

void circToXML(xmlNodePtr pNode, Circle *circ) {
    xmlNodePtr circNode = xmlNewChild(pNode, NULL, BAD_CAST "circle", NULL);
    //TODO this is a bad malloc
    char *temp = malloc((sizeof(char) * 20));
    //Get cx
    sprintf(temp, "%f", circ->cx);
    strcat(temp, circ->units);
    xmlNewProp(circNode, BAD_CAST "cx", BAD_CAST temp);
    //Get cy
    sprintf(temp, "%f", circ->cy);
    strcat(temp, circ->units);
    xmlNewProp(circNode, BAD_CAST "cy", BAD_CAST temp);
    //Get r
    sprintf(temp, "%f", circ->r);
    strcat(temp, circ->units);
    xmlNewProp(circNode, BAD_CAST "r", BAD_CAST temp);
    //Get otherAttr
    ListIterator iter = createIterator(circ->otherAttributes);
    void *elem;
    while((elem = nextElement(&iter))!= NULL) {
        Attribute *a = (Attribute *) elem;
        xmlNewProp(circNode, BAD_CAST a->name, BAD_CAST a->value);
    }

    free(temp);
}

void pathToXML(xmlNodePtr pNode, Path *path) {
    xmlNodePtr pathNode = xmlNewChild(pNode, NULL, BAD_CAST "path", NULL);
    //Other attr
    ListIterator iter = createIterator(path->otherAttributes);
    void *elem;
    while((elem = nextElement(&iter))!= NULL) {
        Attribute *a = (Attribute *) elem;
        xmlNewProp(pathNode, BAD_CAST a->name, BAD_CAST a->value);
    }
    //Data
    xmlNewProp(pathNode, BAD_CAST "d", BAD_CAST path->data);
    

}

void groupToXML(xmlNodePtr pNode, Group *group) {
    xmlNodePtr gNode = xmlNewChild(pNode, NULL, BAD_CAST "g", NULL);
    //Add Attr
    ListIterator iter = createIterator(group->otherAttributes);
    void *elem;
    while((elem = nextElement(&iter))!= NULL) {
        Attribute *a = (Attribute*) elem;
        xmlNewProp(gNode, BAD_CAST a->name, BAD_CAST a->value);
    }

    //Add rect
    iter = createIterator(group->rectangles);
    while((elem = nextElement(&iter))!=NULL) {
        Rectangle *rect = (Rectangle*) elem;
        rectToXML(gNode, rect);
    }

    //Add circle
    iter = createIterator(group->circles);
    while((elem = nextElement(&iter))!=NULL) {
        Circle *circ = (Circle *) elem;
        circToXML(gNode, circ);
    }

    //Add path
    iter = createIterator(group->paths);
    while((elem = nextElement(&iter))!=NULL) {
        Path *path = (Path *) elem;
        pathToXML(gNode, path);
    }

    //Add other groups
    iter = createIterator(group->groups);
    while((elem = nextElement(&iter))!= NULL) {
        Group *g = (Group *) elem;
        groupToXML(gNode, g);
    }


}


//TODO function converting SVG to XMLdoc
xmlDocPtr svgToTree(const SVG *svg) {
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlDocSetRootElement(doc,root_node);

    //Set NS
    xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST svg->namespace, NULL);
    xmlSetNs(root_node, ns);

    //Add attributes of the SVG Node
    ListIterator iter = createIterator(svg->otherAttributes);
    void *elem;
    while((elem = nextElement(&iter))!=NULL) {
        Attribute *a = (Attribute *) elem;
        xmlNewProp(root_node, BAD_CAST a->name, BAD_CAST a->value);

    }

    //Set title and description
    if(strcmp(svg->title, "\0") != 0) {
        xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST svg->title);
    }
    if(strcmp(svg->description, "\0") != 0) {
        xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST svg->description);
    }

    //Add rectangles to tree
    iter = createIterator(svg->rectangles);
    while((elem = nextElement(&iter))!=NULL) {
        Rectangle *rect = (Rectangle *) elem;
        rectToXML(root_node, rect);

    }

    //Add circle
    iter = createIterator(svg->circles);
    while((elem=nextElement(&iter)) != NULL) {
        Circle *circ = (Circle *) elem;
        circToXML(root_node, circ);
    }

    //Add paths
    iter = createIterator(svg->paths);
    while((elem = nextElement(&iter))!= NULL) {
        Path *path = (Path *) elem;
        pathToXML(root_node, path);
    }

    //Add groups
    iter = createIterator(svg->groups);
    while((elem = nextElement(&iter))!= NULL) {
        Group *g = (Group *) elem;
        groupToXML(root_node, g);
    }

    xmlSaveFormatFile("test.svg", doc, 1);

    //xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return doc;
}

/**
 * @brief Validates an xmlDoc, NOT VALUES just in ref to the xsd file
 * 
 * @param fileName .svg filename
 * @param xsdRef .xsd validation file
 * @return int the result of the validateDoc call
 */
int validateTree(xmlDocPtr doc, char *xsdRef) {

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr txt;

    xmlLineNumbersDefault(1);

    txt = xmlSchemaNewParserCtxt(xsdRef);

    xmlSchemaSetParserErrors(txt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(txt);

    xmlSchemaFreeParserCtxt(txt);

    if(doc == NULL) {
        fprintf(stderr, "Could not parse in validateTree\n");
        return -1;
    }

    xmlSchemaValidCtxtPtr ctxt;
    int ret;

    ctxt = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    ret = xmlSchemaValidateDoc(ctxt, doc);

    xmlSchemaFreeValidCtxt(ctxt);


    if(schema !=NULL) {
        xmlSchemaFree(schema);
    }
    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();

    return ret;

}