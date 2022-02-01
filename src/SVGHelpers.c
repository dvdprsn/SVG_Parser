#include "SVGHelpers.h"

void parser(xmlNode *a_node, SVG *svg) {

    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {

        /*
        if(cur_node->parent->type == XML_ELEMENT_NODE && cur_node->type == XML_ELEMENT_NODE) {
            if(xmlStrcmp(cur_node->parent->name, (const xmlChar *)"g") == 0){
                printf("Parent name: %s\n", cur_node->parent->name);
                printf("-------->Child name: %s\n", cur_node->name);
            } else {
                printf("Parent name: %s\n", cur_node->parent->name);
                printf("Child name: %s\n", cur_node->name);
            }
            
            printf("-----------------------\n");
        } else if (cur_node->type == XML_ELEMENT_NODE) {
            printf("Child name: %s\n", cur_node->name);
        }
        */
       char *nodeName = "\0";

        if (cur_node->type == XML_ELEMENT_NODE) {

            nodeName = (char *)cur_node->name;

            if(strcmp(nodeName, "title") == 0) { //Works
                setSVGTitle(svg,cur_node->children);
            } else if (strcmp(nodeName, "desc") == 0) { //Works
                setSVGDesc(svg,cur_node->children);
            } else if (strcmp(nodeName, "svg") == 0) { //Works (Fills it, unsure about mem issues)
                fillSVG(svg,cur_node);
            }
        /*
            if (xmlStrcmp(cur_node->name, (const xmlChar *) "path") == 0) { //Create Path
                createPath(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "circle") == 0) { //Create Circle
                createCircle(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "rectangle") == 0) { //Create Rectangle
                createRect(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "svg") == 0) {
                fillSVG(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "title") == 0) {
                if(cur_node->content != NULL) {
                    printf(" content: %s\n ", cur_node->content);
                }
                //setSVGTitle(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "desc") == 0) {
                setSVGDesc(svg, cur_node);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar *) "g") == 0) {
                createGroup(svg, cur_node);
            }
            */

        }

        parser(cur_node->children, svg);

    }

}

void createRect(SVG *svg, xmlNode *cur_node) {

    Rectangle *rect = malloc(sizeof(Rectangle));
    initRect(rect);
    char *ptr = "\0";
    xmlAttr *attr;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;

        char *attrName = (char *) attr->name;
        char *cont = (char *) (value->content);

        if (strcmp(attrName, "x") == 0) {
            rect->x = strtol(cont, &ptr, 10); //Get X
            strcpy(rect->units, ptr); //Get units
        } else if (strcmp(attrName, "y") == 0) {
            rect->y = strtol(cont, &ptr, 10); //Get Y
        } else if (strcmp(attrName, "width") == 0) {
            rect->width = strtol(cont, &ptr, 10); //Get Width
        } else if (strcmp(attrName, "height") == 0) {
            rect->height = strtol(cont, &ptr, 10); //Get Height
        } else { //Other Attributes
            insertBack(rect->otherAttributes, createAttr(attrName, cont));
        }

    }

    insertBack(svg->rectangles, rect);

}

void createCircle(SVG *svg, xmlNode *cur_node) {
    Circle *circle = malloc(sizeof(Circle));
    initCircle(circle);
    char *ptr = "\0";
    xmlAttr *attr;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;

        char *attrName = (char *) attr->name;
        char *cont = (char *) (value->content);

        if (strcmp(attrName, "cx") == 0) {
            circle->cx = strtol(cont, &ptr, 10); //Get X
            strcpy(circle->units, ptr); //Get units
        } else if (strcmp(attrName, "cy") == 0) {
            circle->cy = strtol(cont, &ptr, 10); //Get Y
        } else if (strcmp(attrName, "r") == 0) {
            circle->r = strtol(cont, &ptr, 10); //Get Width
        } else { //Other Attributes
            insertBack(circle->otherAttributes, createAttr(attrName, cont));
        }

    }

    insertBack(svg->circles, circle);

}

void createGroup(SVG *svg, xmlNode *cur_node) { //TODO Figure out groups (nested groups specifically)
    Group *group = malloc(sizeof(Group));
    initGroup(group);
}

Attribute *createAttr(char *name, char value[]) {

    Attribute *attr;
    attr = malloc(sizeof(Attribute) + sizeof(char) * strlen(value)); //Allocate for flexible array member
    if(attr == NULL) return NULL; //If alloc failed

    attr->name = malloc(sizeof(char)*strlen(name)); //Allocate space for the char* in attr struct
    if(attr->name == NULL) return NULL; //Check for fail
    
    strcpy(attr->name, name);
    strcpy(attr->value, value); //Copy string for value

    printf("createAttr -> Name: %s, Value: %s\n",attr->name,attr->value);

    return attr;    //Return create attribute to caller

}

Path *createPath(SVG *svg, xmlNode *cur_node) {
    xmlAttr *attr;
    Path *path;

    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;

        char *attrName = (char *) attr->name;
        char *cont = (char *) (value->content);

        if (strcmp(attrName, "d") == 0) {
            //TODO may not malloc depending on order in path
            path = malloc(sizeof(Path) + sizeof(char) * strlen(cont));
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
    circle->otherAttributes = initializeList(&circleToString, &deleteCircle, &compareCircles);
}

void initRect(Rectangle *rect) {
    rect->height = 0;
    rect->width = 0;
    rect->x = 0;
    rect->y = 0;
    strcpy(rect->units, "\0");
    rect->otherAttributes = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
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
