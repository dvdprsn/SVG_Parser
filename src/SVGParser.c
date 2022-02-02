#include "SVGHelpers.h"
#include "SVGParser.h"

SVG* createSVG(const char *fileName) {

    xmlDoc *doc = NULL;
    xmlNode *root = NULL;
    //Creates Empty SVG struct
    SVG *svgReturn = malloc(sizeof(SVG));
    if(svgReturn == NULL) {
        return NULL;
    }

    initSVG(svgReturn);

    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("Error: Failed to parse");
    }

    root = xmlDocGetRootElement(doc);

    strcpy(svgReturn->namespace,getNS(root));

    parser(root, svgReturn);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return svgReturn;
}

char *SVGToString(const SVG *img) {
    int charSize = 30;
    char *toReturn = malloc(sizeof(char)*charSize);
    
    if(strcmp(img->namespace,"\0")!=0) {
        charSize += strlen(img->namespace);
        toReturn = realloc(toReturn,sizeof(char)*charSize+20);
        strcat(toReturn, "NS: ");
        strcat(toReturn, img->namespace);
        strcat(toReturn, "\n");

    }
    if(strcmp(img->title,"\0")!= 0) {
        charSize += strlen(img->title);
        toReturn = realloc(toReturn,sizeof(char)*charSize+20);
        strcat(toReturn, "Title: ");
        strcat(toReturn, img->title);
        strcat(toReturn, "\n");

    }
    if(strcmp(img->description, "\0")!=0) {
        charSize += strlen(img->description);
        toReturn = realloc(toReturn,sizeof(char)*charSize+20);
        strcat(toReturn, "Description: ");
        strcat(toReturn, img->description);
        strcat(toReturn, "\n");
    }
    strcat(toReturn, "------Rect------\n");

    charSize += strlen(toString(img->rectangles));
    toReturn = realloc(toReturn, sizeof(char)*charSize+20);
    if(toReturn == NULL){
        fprintf(stderr, "REALLOCE FAILED");
    }

    strcat(toReturn, toString(img->rectangles));
    
    
    return toReturn;

}

void deleteSVG(SVG *img) {
    
    //freeList(img->otherAttributes);
    //freeList(img->paths);
    //freeList(img->rectangles);
    /*
    freeList(img->circles);
    freeList(img->rectangles);
    freeList(img->groups);
     */
    //TODO you can just call free list once on each attribute of the SVG struct sine we pass in the delete for each data type
    //TODO so long as the delete functions are correct
    free(img);
}

//-------------------GETTERS----------------------------------

// Function that returns a list of all rectangles in the struct.
List *getRects(const SVG *img) {
    
}

// Function that returns a list of all circles in the struct.
List *getCircles(const SVG *img) {
    
}

// Function that returns a list of all groups in the struct.
List *getGroups(const SVG *img) {
    
}

// Function that returns a list of all paths in the struct.
List *getPaths(const SVG *img) {
    
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
//ATTRIBUTE
void deleteAttribute(void *data) {
    //Much of this logic is from the StructListDemo
    Attribute* tmpAttr; // Create temp attribute

    if(data == NULL)  { //Check if empty already
        return;
    }

    tmpAttr = (Attribute *)data; // Cast data to type attribute

    //free(tmpAttr->name);
    //free(tmpAttr);

}

char *attributeToString(void *data) { 

    char *tmpStr;
    Attribute *tmpAttr;
    int len;

    if(data == NULL) return NULL;
    tmpAttr = (Attribute *) data;

    len = strlen(tmpAttr->name) + strlen(tmpAttr->value);
    tmpStr = malloc(sizeof(char)*len); //MUST BE FREED AFTER USE
    if(tmpStr == NULL) return NULL;

    sprintf(tmpStr, "Name: %s, Value %s", tmpAttr->name, tmpAttr->value);

    return tmpStr;

}

int compareAttributes(const void *first, const void *second) {
    //These can be empty stubs
    return -1;
}

//GROUPS
void deleteGroup(void *data) {
    Group *tmpGroup; 

    if(data == NULL) {
        return;
    }

    tmpGroup = (Group*) data;


}

char *groupToString(void *data) {

}

int compareGroups(const void *first, const void *second) {
    return -1;
}

//RECTANGLE
void deleteRectangle(void *data) {

}

char *rectangleToString(void *data) {

    char *tmpStr;
    Rectangle *tmpRect;
    int len;
    if(data == NULL) return NULL;

    tmpRect = (Rectangle *) data;

    tmpStr = malloc(sizeof(char)*256);
    if(tmpStr == NULL) return NULL;

    sprintf(tmpStr, "x: %f y: %f units: %s, width: %f height: %f ",tmpRect->x, tmpRect->y, tmpRect->units, tmpRect->width, tmpRect->height);
    //printf("%s\n",tmpStr);
    strcat(tmpStr, "\n");
    printf("tmSt: %lu\n",strlen(tmpStr)*sizeof(char));

    /tmpStr = realloc(tmpStr, sizeof(char)*(strlen(toString(tmpRect->otherAttributes))+30));
    if(tmpStr == NULL) {
        fprintf(stderr, "Error: ");
    }
    printf("Inital alloc: %lu\n", sizeof(char)*256);
    printf("Space taken: %lu\n",strlen(tmpStr)*sizeof(char));
    printf("Additional needed: %lu\n", sizeof(char)*(strlen(toString(tmpRect->otherAttributes))+30));
    //strcat(tmpStr, toString(tmpRect->otherAttributes));
    //printf("%s",toString(tmpRect->otherAttributes));

    return tmpStr;

}

int compareRectangles(const void *first, const void *second) {
    return -1;
}

//CIRCLE
void deleteCircle(void *data) {

}

char *circleToString(void *data) {

}

int compareCircles(const void *first, const void *second) {
    return -1;
}

//PATHS
void deletePath(void *data) {

    //Much of this logic is from the StructListDemo

    Path *tmpPath;

    if(data == NULL)  { //Check if empty already
        return;
    }

    tmpPath = (Path *)data; // Cast data to type attribute

    //free(tmpPath);

}

char *pathToString(void *data) {
/*
    char *tmpStr;
    Attribute *tmpAttr;
    int len;

    if(data == NULL) return NULL;
    tmpAttr = (Attribute *) data;

    len = strlen(tmpAttr->name) + strlen(tmpAttr->value);
    tmpStr = malloc(sizeof(char)*len); //MUST BE FREED AFTER USE
    if(tmpStr == NULL) return NULL;

    sprintf(tmpStr, "Name: %s, Value %s", tmpAttr->name, tmpAttr->value);

    return tmpStr;
*/

    char *tmpStr;
    Path *tmpPath;
    int len;
    if(data == NULL) return NULL;

    tmpPath = (Path *) data;

    len = strlen(tmpPath->data);

    tmpStr = malloc(sizeof(char)*len);
    if(tmpStr == NULL) return NULL;

    sprintf(tmpStr, "Data: %s", tmpPath->data);
    strcat(tmpStr, "\n");
    strcat(tmpStr, toString(tmpPath->otherAttributes));
    return tmpStr;

}

int comparePaths(const void *first, const void *second) {
    return -1;
}
