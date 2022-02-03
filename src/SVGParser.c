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
    char *sizeCheck;
    char *toReturn = malloc(sizeof(char)*charSize);
    strcpy(toReturn, "--RUNNING--\n");
    
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
        toReturn = realloc(toReturn,sizeof(*toReturn)*(charSize+20));
        strcat(toReturn, "Description: ");
        strcat(toReturn, img->description);
        strcat(toReturn, "\n");
    }
    //RECTANGLE
    charSize += strlen("------Rect------\n")+20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "------Rect------\n");

    sizeCheck = toString(img->rectangles);
    charSize += strlen(sizeCheck)+20;
    toReturn = realloc(toReturn, sizeof(char)*charSize+20);
    if(toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    //CIRCLE
    charSize += strlen("----Circle----\n")+20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "----Circle----\n");
    
    sizeCheck = toString(img->circles);
    charSize += strlen(sizeCheck)+20;
    toReturn = realloc(toReturn, sizeof(*toReturn)*(charSize));
    if(toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    //PATH
    charSize += strlen("---PATH----\n")+20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "---PATH----\n");

    sizeCheck = toString(img->paths);
    charSize += strlen(sizeCheck)+20;
    toReturn = realloc(toReturn, sizeof(*toReturn)*charSize);
    if(toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    //GROUP
    sizeCheck = toString(img->groups);
    charSize += strlen(sizeCheck)+20;
    toReturn = realloc(toReturn, sizeof(*toReturn)*(charSize));
    if(toReturn == NULL) return NULL;
    strcat(toReturn,sizeCheck);

    free(sizeCheck);
    
    return toReturn;

}

void deleteSVG(SVG *img) {
    
    freeList(img->otherAttributes);
    freeList(img->paths);
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->groups);

    free(img);
}

//-------------------GETTERS----------------------------------
//Create new list and iterate through the list in img to fill

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

    free(tmpAttr->name);
    free(tmpAttr);

}

char *attributeToString(void *data) { 

    char *tmpStr;
    Attribute *tmpAttr;
    int len;

    if(data == NULL) return NULL;
    tmpAttr = (Attribute *) data;

    len = strlen(tmpAttr->name) + strlen(tmpAttr->value)+2;
    tmpStr = malloc(sizeof(*tmpStr)*len+100); //MUST BE FREED AFTER USE
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
    //NOTE - Free lists first then freeList on the groups (Check valgrind)
    Group *tmpGroup; 

    if(data == NULL) {
        return;
    }

    tmpGroup = (Group *) data;

    freeList(tmpGroup->circles);
    freeList(tmpGroup->paths);
    freeList(tmpGroup->rectangles);
    freeList(tmpGroup->otherAttributes);

    freeList(tmpGroup->groups); //Should trigger recursion since freeList uses this function

    free(tmpGroup); //On exit of recursion we can free the group
}

char *groupToString(void *data) {
    char *tmpStr;
    char *strTest;

    int tmpSize = 30;

    Group *tmpGroup;
    if(data == NULL) return NULL;
    tmpGroup = (Group *)data;

    //Group Separator
    tmpSize += strlen("-------GROUP------\n") +30;
    tmpStr = malloc(sizeof(*tmpStr)*tmpSize);
    strcpy(tmpStr,"-------GROUP------\n");

    //Grab rectangles
    strTest = toString(tmpGroup->rectangles);
    tmpSize += strlen(strTest)+30;
    tmpStr = realloc(tmpStr,sizeof(*tmpStr)*tmpSize);
    strcat(tmpStr,strTest);
    free(strTest);

    //GrabCircles
    strTest = toString(tmpGroup->circles);
    tmpSize += strlen(strTest)+ 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr)*tmpSize);
    strcat(tmpStr,strTest);
    free(strTest);

    //Grab Paths
    strTest = toString(tmpGroup->paths);
    tmpSize += strlen(strTest)+30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr)*tmpSize);
    strcat(tmpStr,strTest);
    free(strTest);

    //Grab Attributes
    strTest = toString(tmpGroup->otherAttributes);
    tmpSize += strlen(strTest)+30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr)*tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    //Recursive Groups
    strTest = toString(tmpGroup->groups);
    tmpSize += strlen(strTest)+30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr)*tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    //Return
    return tmpStr;


}

int compareGroups(const void *first, const void *second) {
    return -1;
}

//RECTANGLE
void deleteRectangle(void *data) {

    Rectangle *tmpRect;
    if(data == NULL) return;
    tmpRect = (Rectangle *) data;
    freeList(tmpRect->otherAttributes);
    free(tmpRect);

}

char *rectangleToString(void *data) {

    char *tmpStr;
    char *strTest;
    Rectangle *tmpRect;
    if(data == NULL) return NULL;

    tmpRect = (Rectangle *) data;

    tmpStr = malloc(sizeof(char)*256);
    if(tmpStr == NULL) return NULL;

    sprintf(tmpStr, "x: %f y: %f units: %s, width: %f height: %f ",tmpRect->x, tmpRect->y, tmpRect->units, tmpRect->width, tmpRect->height);
    strcat(tmpStr, "\n");

    strTest = toString(tmpRect->otherAttributes);

    tmpStr = realloc(tmpStr, sizeof(char)*(strlen(strTest)+30+sizeof(char)*256));
    if(tmpStr == NULL) {
        fprintf(stderr, "Error: ");
    }
    strcat(tmpStr, strTest);
    strcat(tmpStr,"\n");

    free(strTest);

    return tmpStr;

}

int compareRectangles(const void *first, const void *second) {
    return -1;
}

//CIRCLE
void deleteCircle(void *data) {
    Circle *tmpCirc;

    if(data == NULL) return;

    tmpCirc = (Circle *) data;

    freeList(tmpCirc->otherAttributes);
    free(tmpCirc);



}

char *circleToString(void *data) {
    char *tmpStr;
    char *sizeTest;
    Circle *tmpCirc;

    if(data == NULL) return NULL;

    tmpCirc = (Circle *) data;
    tmpStr = malloc(sizeof(*tmpStr)*256);

    if(tmpCirc == NULL) {
        return NULL;

    }

    sizeTest = toString(tmpCirc->otherAttributes);

    sprintf(tmpStr,"cx: %f cy: %f r: %f units: %s", tmpCirc->cx, tmpCirc->cy, tmpCirc->r, tmpCirc->units);

    tmpStr = realloc(tmpStr, sizeof(*tmpStr)*(strlen(sizeTest)+30+sizeof(char)*256));
    if(tmpStr == NULL) return NULL;

    strcat(tmpStr,sizeTest);
    strcat(tmpStr, "\n");

    free(sizeTest);

    return tmpStr;
}

int compareCircles(const void *first, const void *second) {
    return -1;
}

//PATHS
void deletePath(void *data) {

    Path *tmpPath;

    if(data == NULL)  { //Check if empty already
        return;
    }

    tmpPath = (Path *)data; 
    freeList(tmpPath->otherAttributes);
    free(tmpPath);

}

char *pathToString(void *data) { 
    char *tmpStr;
    Path *tmpPath;
    char *strTest;
    int len;

    if(data == NULL) return NULL;

    tmpPath = (Path *) data;

    len = strlen(tmpPath->data);

    tmpStr = malloc(sizeof(char)*(len+256));
    if(tmpStr == NULL) return NULL;

    strTest = toString(tmpPath->otherAttributes);

    sprintf(tmpStr, "Data: %s", tmpPath->data);
    strcat(tmpStr, strTest);
    strcat(tmpStr, "\n");

    free(strTest);

    return tmpStr;

}

int comparePaths(const void *first, const void *second) {
    return -1;
}
