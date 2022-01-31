#include "SVGHelpers.h"
#include "SVGParser.h"

//TODO Ask about when we need to check for contents

SVG* createSVG(const char *fileName) {
    //TODO Lecture 2B is super useful
    //TODO for path attr 'd' it is just a single string, doesnt need to be parsed into elements
    xmlDoc *doc = NULL;
    xmlNode *root = NULL;
    //Creates Empty SVG struct
    //TODO Catch for failed malloc
    //TODO Namespace has a special way to be pulled out
    SVG *svgReturn = malloc(sizeof(SVG));
    //Init all linked lists associated with elements of this SVG
    //TODO other attributes must be set, can be empty, NEVER null
    //TODO Create a function in helper to init svg
    //TODO Check StructListDemo for how to use these
    initSVG(svgReturn);

    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("Error: Failed to parse");
    }

    root = xmlDocGetRootElement(doc);

    strcpy(svgReturn->namespace,getNS(root));

    parser(root, svgReturn);
    //TODO: Use parser functions for other attributes here

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return svgReturn;
}

char *SVGToString(const SVG *img) {
    
    printf("Namespace: %s\n", img->namespace);

}

void deleteSVG(SVG *img) {
    /*
    freeList(img->otherAttributes);
    freeList(img->circles);
    freeList(img->rectangles);
    freeList(img->groups);
     */
    //TODO you can just call free list once on each attribute of the SVG struct sine we pass in the delete for each data type
    //TODO so long as the delete functions are correct
    //TODO Free Attributes and list
    free(img);
}

//-------------------GETTERS----------------------------------

// Function that returns a list of all rectangles in the struct.
List *getRects(const SVG *img) {
    return img->rectangles;
}

// Function that returns a list of all circles in the struct.
List *getCircles(const SVG *img) {
    return img->circles;
}

// Function that returns a list of all groups in the struct.
List *getGroups(const SVG *img) {
    return img->groups;
}

// Function that returns a list of all paths in the struct.
List *getPaths(const SVG *img) {
    return img->paths;
}

//-------------------SEARCHERS--------------------------
// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(const SVG *img, float area) {
    return 0;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(const SVG *img, float area) {
    return 0;
}

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(const SVG *img, const char *data) {
    return 0;
}

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(const SVG *img, int len) {
    return 0;
}

int numAttr(const SVG *img) {
    return 0;
}

//------------------------HELPER--------------------------------
//TODO These helpers are passed into the LIST API
//ATTRIBUTE
void deleteAttribute(void *data) {
    //Make sure it is not null
    //See structlistdemo.c for help
    free(data);
}

char *attributeToString(void *data) {   

}

int compareAttributes(const void *first, const void *second) {
    //These can be empty stubs
    return -1;
}

//GROUPS
void deleteGroup(void *data) {
    free(data);
    //Should likely be reccursive
}

char *groupToString(void *data) {

}

int compareGroups(const void *first, const void *second) {

}

//RECTANGLE
void deleteRectangle(void *data) {
    free(data);
}

char *rectangleToString(void *data) {

}

int compareRectangles(const void *first, const void *second) {
    return -1;
}

//CIRCLE
void deleteCircle(void *data) {
    free(data);
}

char *circleToString(void *data) {

}

int compareCircles(const void *first, const void *second) {
    return -1;
}

//PATHS
void deletePath(void *data) {
    free(data);
}

char *pathToString(void *data) {

}

int comparePaths(const void *first, const void *second) {
    return -1;
}
