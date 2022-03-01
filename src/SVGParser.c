/**
 * @file SVGParser.c
 * @author David Pearson (1050197)
 * @brief
 *
 */

#include "SVGParser.h"

#include "SVGHelpers.h"

SVG *createSVG(const char *fileName) {
    xmlDoc *doc = NULL;
    xmlNode *root = NULL;
    // Creates Empty SVG struct + buffer
    SVG *svgReturn = malloc(sizeof(SVG) + 30);
    if (svgReturn == NULL) {
        return NULL;
    }

    initSVG(svgReturn);  // Fill svg with blanks
    // Read XML from file
    doc = xmlReadFile(fileName, NULL, 0);
    if (doc == NULL) return NULL;
    // Find XML root
    root = xmlDocGetRootElement(doc);
    if (root == NULL) return NULL;
    // Copy namespace to svg struct
    strcpy(svgReturn->namespace, getNS(root));
    // Send to parser to complete svg struct
    parser(root, svgReturn);
    // Free the doc and clean parser
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return svgReturn;
}

char *SVGToString(const SVG *img) {
    int charSize = 30;
    char *sizeCheck;
    char *toReturn = malloc(sizeof(char) * charSize);
    strcpy(toReturn, "--RUNNING toString--\n");

    if (strcmp(img->namespace, "\0") != 0) {
        charSize += strlen(img->namespace);
        toReturn = realloc(toReturn, sizeof(char) * charSize + 20);
        strcat(toReturn, "NS: ");
        strcat(toReturn, img->namespace);
        strcat(toReturn, "\n");
    }
    if (strcmp(img->title, "\0") != 0) {
        charSize += strlen(img->title);
        toReturn = realloc(toReturn, sizeof(char) * charSize + 20);
        strcat(toReturn, "Title: ");
        strcat(toReturn, img->title);
        strcat(toReturn, "\n");
    }
    if (strcmp(img->description, "\0") != 0) {
        charSize += strlen(img->description);
        toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize + 20));
        strcat(toReturn, "Description: ");
        strcat(toReturn, img->description);
        strcat(toReturn, "\n");
    }
    // RECTANGLE
    charSize += strlen("------Rect------\n") + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "------Rect------\n");

    sizeCheck = toString(img->rectangles);
    charSize += strlen(sizeCheck) + 20;
    toReturn = realloc(toReturn, sizeof(char) * charSize + 20);
    if (toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    // CIRCLE
    charSize += strlen("----Circle----\n") + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "----Circle----\n");

    sizeCheck = toString(img->circles);
    charSize += strlen(sizeCheck) + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    if (toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    // PATH
    charSize += strlen("---PATH----\n") + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    strcat(toReturn, "---PATH----\n");

    sizeCheck = toString(img->paths);
    charSize += strlen(sizeCheck) + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * charSize);
    if (toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    // GROUP
    sizeCheck = toString(img->groups);
    charSize += strlen(sizeCheck) + 20;
    toReturn = realloc(toReturn, sizeof(*toReturn) * (charSize));
    if (toReturn == NULL) return NULL;
    strcat(toReturn, sizeCheck);

    free(sizeCheck);

    return toReturn;
}

void deleteSVG(SVG *img) {
    if (img == NULL) return;
    // Free all lists
    freeList(img->otherAttributes);
    freeList(img->paths);
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->groups);
    // Finally free svg struct itself
    free(img);
}

//-------------------GETTERS----------------------------------
// Create new list and iterate through the list in img to fill

// Function that returns a list of all rectangles in the struct.
List *getRects(const SVG *img) {
    if (img == NULL) return NULL;

    List *toReturn = initializeList(&rectangleToString, &dummyDel, &compareRectangles);
    ListIterator iter = createIterator(img->rectangles);
    void *elem;
    // Get rectangles from rectangle list in SVG struct
    while ((elem = nextElement(&iter)) != NULL) {
        Rectangle *rec = (Rectangle *)elem;
        insertBack(toReturn, rec);
    }

    // Get rectangles from groups and subgroups within SVG

    iter = createIterator(img->groups);

    while ((elem = nextElement(&iter)) != NULL) {
        Group *grp = (Group *)elem;
        findRect(grp, toReturn);
    }

    return toReturn;
}

// Function that returns a list of all circles in the struct.
List *getCircles(const SVG *img) {
    if (img == NULL) return NULL;
    // Create list with dummy delete to prevent double frees
    List *toReturn = initializeList(&circleToString, &dummyDel, &compareCircles);

    // Get from circles list

    ListIterator iter = createIterator(img->circles);

    void *elem;
    while ((elem = nextElement(&iter)) != NULL) {
        Circle *circ = (Circle *)elem;
        insertBack(toReturn, circ);
    }

    // GET from groups and subgroups
    iter = createIterator(img->groups);
    while ((elem = nextElement(&iter)) != NULL) {
        Group *group = (Group *)elem;
        findCirc(group, toReturn);
    }

    return toReturn;
}

// Function that returns a list of all groups in the struct.
List *getGroups(const SVG *img) {
    if (img == NULL) return NULL;
    List *toReturn = initializeList(&groupToString, &dummyDel, &compareGroups);

    ListIterator iter = createIterator(img->groups);

    void *elem;
    while ((elem = nextElement(&iter)) != NULL) {
        Group *group = (Group *)elem;
        insertBack(toReturn, group);
        findGroup(group, toReturn);  // Find subgroups of group just added
    }

    return toReturn;
}

// Function that returns a list of all paths in the struct.
List *getPaths(const SVG *img) {
    if (img == NULL) return NULL;
    List *toReturn = initializeList(&pathToString, &dummyDel, &compareCircles);

    ListIterator iter = createIterator(img->paths);

    void *elem;
    while ((elem = nextElement(&iter)) != NULL) {
        Path *path = (Path *)elem;
        insertBack(toReturn, path);
    }
    // Get from groups
    iter = createIterator(img->groups);
    while ((elem = nextElement(&iter)) != NULL) {
        Group *group = (Group *)elem;
        findPaths(group, toReturn);
    }

    return toReturn;
}

//-------------------SEARCHERS--------------------------
// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(const SVG *img, float area) {
    if (img == NULL) return -1;

    List *rectList = getRects(img);
    ListIterator iter = createIterator(rectList);
    void *elem;

    float ar = 0;
    int toReturn = 0;

    while ((elem = nextElement(&iter)) != NULL) {
        Rectangle *rect = (Rectangle *)elem;
        ar = rect->height * rect->width;
        ar = ceil(ar);
        area = ceil(area);
        if (ar == area) {
            toReturn++;
        }
    }

    freeList(rectList);
    return toReturn;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(const SVG *img, float area) {
    if (img == NULL) return -1;

    List *circList = getCircles(img);
    ListIterator iter = createIterator(circList);
    void *elem;

    float ar = 0;
    int toReturn = 0;

    while ((elem = nextElement(&iter)) != NULL) {
        Circle *circ = (Circle *)elem;
        ar = (circ->r * circ->r) * PI;
        ar = ceil(ar);
        area = ceil(area);

        if (ar == area) {
            toReturn++;
        }
    }

    freeList(circList);
    return toReturn;
    ;
}

// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(const SVG *img, const char *data) {
    if (img == NULL) return -1;

    List *pathList = getPaths(img);
    ListIterator iter = createIterator(pathList);
    void *elem;
    int match = 0;

    while ((elem = nextElement(&iter)) != NULL) {
        Path *path = (Path *)elem;
        if (strcmp(path->data, data) == 0) {
            match++;
        }
    }

    freeList(pathList);
    return match;
}

// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(const SVG *img, int len) {
    if (img == NULL) return -1;

    int matches = 0;
    int lenOfGroup = 0;

    List *groups = getGroups(img);
    ListIterator iter = createIterator(groups);
    void *elem;

    while ((elem = nextElement(&iter)) != NULL) {
        lenOfGroup = 0;
        Group *group = (Group *)elem;
        lenOfGroup += getLength(group->circles);
        lenOfGroup += getLength(group->rectangles);
        lenOfGroup += getLength(group->paths);
        lenOfGroup += getLength(group->groups);

        if (lenOfGroup == len) {
            matches++;
        }
    }

    return matches;
}

int numAttr(const SVG *img) {
    if (img == NULL) return -1;
    void *elem;
    int numAttr = 0;

    // Get num attributes from SVG
    numAttr += getLength(img->otherAttributes);

    // Attributes in rectangles list
    List *rects = getRects(img);
    ListIterator iter = createIterator(rects);
    while ((elem = nextElement(&iter)) != NULL) {
        Rectangle *rect = (Rectangle *)elem;
        numAttr += getLength(rect->otherAttributes);
    }
    freeList(rects);

    // Attributes in circles list
    List *circles = getCircles(img);
    iter = createIterator(circles);
    while ((elem = nextElement(&iter)) != NULL) {
        Circle *circ = (Circle *)elem;
        numAttr += getLength(circ->otherAttributes);
    }
    freeList(circles);

    // Attributes in paths list

    List *paths = getPaths(img);
    iter = createIterator(paths);
    while ((elem = nextElement(&iter)) != NULL) {
        Path *path = (Path *)elem;
        numAttr += getLength(path->otherAttributes);
    }
    freeList(paths);

    // Attributes in groups list
    List *groups = getGroups(img);
    iter = createIterator(groups);
    while ((elem = nextElement(&iter)) != NULL) {
        Group *group = (Group *)elem;
        numAttr += getLength(group->otherAttributes);
    }
    freeList(groups);

    return numAttr;
}

//------------------------HELPER--------------------------------
// ATTRIBUTE
void deleteAttribute(void *data) {
    Attribute *tmpAttr;  // Create temp attribute

    if (data == NULL) return;

    tmpAttr = (Attribute *)data;  // Cast data to type attribute

    free(tmpAttr->name);  // Free name char
    free(tmpAttr);        // Free attribute struct
}

char *attributeToString(void *data) {
    char *tmpStr;
    Attribute *tmpAttr;
    int len;

    if (data == NULL) return NULL;
    tmpAttr = (Attribute *)data;

    len = strlen(tmpAttr->name) + strlen(tmpAttr->value) + 20;
    // Alloc space for output plus a buffer
    tmpStr = malloc(sizeof(*tmpStr) * len + 100);
    if (tmpStr == NULL) return NULL;

    sprintf(tmpStr, "Name: %s, Value %s", tmpAttr->name, tmpAttr->value);

    return tmpStr;
}

int compareAttributes(const void *first, const void *second) {
    // These can be empty stubs
    return -1;
}

// GROUPS
void deleteGroup(void *data) {
    // NOTE - Free lists first then freeList on the groups (Check valgrind)
    Group *tmpGroup;

    if (data == NULL) {
        return;
    }

    tmpGroup = (Group *)data;
    // Free all lists within this group
    freeList(tmpGroup->circles);
    freeList(tmpGroup->paths);
    freeList(tmpGroup->rectangles);
    freeList(tmpGroup->otherAttributes);

    freeList(tmpGroup->groups);  // Should trigger recursion since freeList uses this function

    free(tmpGroup);  // On exit of recursion we can free the group
}

char *groupToString(void *data) {
    char *tmpStr;
    char *strTest;

    int tmpSize = 30;

    Group *tmpGroup;
    if (data == NULL) return NULL;
    tmpGroup = (Group *)data;

    // Group Separator
    tmpSize += strlen("-------GROUP------\n") + 30;
    tmpStr = malloc(sizeof(*tmpStr) * tmpSize);
    strcpy(tmpStr, "-------GROUP------\n");

    // Grab rectangles
    strTest = toString(tmpGroup->rectangles);
    tmpSize += strlen(strTest) + 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    // GrabCircles
    strTest = toString(tmpGroup->circles);
    tmpSize += strlen(strTest) + 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    // Grab Paths
    strTest = toString(tmpGroup->paths);
    tmpSize += strlen(strTest) + 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    // Grab Attributes
    strTest = toString(tmpGroup->otherAttributes);
    tmpSize += strlen(strTest) + 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    // Recursive Groups
    strTest = toString(tmpGroup->groups);
    tmpSize += strlen(strTest) + 30;
    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * tmpSize);
    strcat(tmpStr, strTest);
    free(strTest);

    // Return
    return tmpStr;
}

int compareGroups(const void *first, const void *second) {
    return -1;
}

// RECTANGLE
void deleteRectangle(void *data) {
    if (data == NULL) return;

    Rectangle *tmpRect;
    tmpRect = (Rectangle *)data;
    freeList(tmpRect->otherAttributes);
    free(tmpRect);
}

char *rectangleToString(void *data) {
    char *tmpStr;
    char *strTest;
    Rectangle *tmpRect;
    if (data == NULL) return NULL;

    tmpRect = (Rectangle *)data;

    tmpStr = malloc(sizeof(*tmpStr) * 256);
    if (tmpStr == NULL) return NULL;

    sprintf(tmpStr, "x: %f y: %f units: %s, width: %f height: %f ", tmpRect->x, tmpRect->y, tmpRect->units, tmpRect->width, tmpRect->height);
    strcat(tmpStr, "\n");

    strTest = toString(tmpRect->otherAttributes);

    tmpStr = realloc(tmpStr, sizeof(char) * (strlen(strTest) + 30 + sizeof(char) * 256));
    if (tmpStr == NULL) {
        fprintf(stderr, "Error: ");
    }
    strcat(tmpStr, strTest);
    strcat(tmpStr, "\n");

    free(strTest);

    return tmpStr;
}

int compareRectangles(const void *first, const void *second) {
    return -1;
}

// CIRCLE
void deleteCircle(void *data) {
    if (data == NULL) return;
    Circle *tmpCirc;

    tmpCirc = (Circle *)data;

    freeList(tmpCirc->otherAttributes);
    free(tmpCirc);
}

char *circleToString(void *data) {
    char *tmpStr;
    char *sizeTest;
    Circle *tmpCirc;

    if (data == NULL) return NULL;

    tmpCirc = (Circle *)data;
    tmpStr = malloc(sizeof(*tmpStr) * 256);

    if (tmpCirc == NULL) {
        return NULL;
    }

    sizeTest = toString(tmpCirc->otherAttributes);

    sprintf(tmpStr, "cx: %f cy: %f r: %f units: %s", tmpCirc->cx, tmpCirc->cy, tmpCirc->r, tmpCirc->units);

    tmpStr = realloc(tmpStr, sizeof(*tmpStr) * (strlen(sizeTest) + 30 + sizeof(char) * 256));
    if (tmpStr == NULL) return NULL;

    strcat(tmpStr, sizeTest);
    strcat(tmpStr, "\n");

    free(sizeTest);

    return tmpStr;
}

int compareCircles(const void *first, const void *second) {
    return -1;
}

// PATHS
void deletePath(void *data) {
    Path *tmpPath;

    if (data == NULL) {  // Check if empty already
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

    if (data == NULL) return NULL;

    tmpPath = (Path *)data;

    len = strlen(tmpPath->data);

    tmpStr = malloc(sizeof(char) * (len + 256));
    if (tmpStr == NULL) return NULL;

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

//------------A2-----------

// MODULE 1

/**
 * @brief Create a Valid SVG Object, validating from schemaFile
 *
 * @param fileName Name of the .svg file
 * @param schemaFile Name of .xsd file
 * @return SVG* Parsed SVG Struct
 */
SVG *createValidSVG(const char *fileName, const char *schemaFile) {
    xmlDocPtr doc = xmlReadFile(fileName, NULL, 0);
    int ret = validateTree(doc, (char *)schemaFile);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    if (ret == 0) {
        // Valid XML
        return createSVG(fileName);
    } else if (ret > 0) {
        // Invalid XML
        return NULL;
    } else {
        // Internal Error
        return NULL;
    }
}

/**
 * @brief Validates the SVG object against schema and constraints
 *
 * @param img SVG img to validate
 * @param schemaFile name of the schemaFile to check against
 * @return true If vaid
 * @return false If invalid
 */
bool validateSVG(const SVG *img, const char *schemaFile) {
    // Validate XML against schema
    xmlDocPtr doc = svgToTree(img);
    int ret = validateTree(doc, (char *)schemaFile);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    validateContents((SVG *)img);

    if (ret == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Outputs the SVG object to a .svg XML file
 *
 * @param img The SVG object to be output
 * @param fileName the name of the file to be created
 * @return true If write was success
 * @return false If write failed
 */
bool writeSVG(const SVG *img, const char *fileName) {
    xmlDocPtr doc = svgToTree(img);
    // TODO Validate writeout file
    int ret = xmlSaveFormatFileEnc((char *)fileName, doc, "UTF-8", 1);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    if (ret > 0) {
        return true;
    } else {
        return false;
    }
}

// MODULE 2
// TODO return here might be wrong
bool setAttribute(SVG *img, elementType elemType, int elemIndex, Attribute *newAttribute) {
}

void addComponent(SVG *img, elementType type, void *newElement) {
}

// MODULE 3

char *attrToJSON(const Attribute *a) {
    char *txt = malloc(sizeof(char) * (strlen(a->name) + strlen(a->value)) + 20);
    if (a == NULL) {
        sprintf(txt, "{}");
        return txt;
    }

    sprintf(txt, "{\"name:\":\"%s\",\"value\":\"%s\"}", a->name, a->value);
    printf("%s\n", txt);

    return txt;
}

char *circleToJSON(const Circle *c) {
    //! Bad Malloc
    char *txt = malloc(sizeof(char) * 200);
    if (c == NULL) {
        sprintf(txt, "{}");
        return txt;
    }
    sprintf(txt, "{\"cx\":%f,\"cy\":%f,\"r\":%f,\"numAttr\":%d,\"units\":\"%s\"}", c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units);

    return txt;
}

char *rectToJSON(const Rectangle *r) {
    //! bad Malloc
    char *txt = malloc(sizeof(char) * 200);
    if (r == NULL) {
        sprintf(txt, "{}");
        return txt;
    }
    sprintf(txt, "{\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f,\"numAttr\":%d,\"units\":\"%s\"}", 
    r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units);

    return txt;
}

char *pathToJSON(const Path *p) {
    
}

char *groupToJSON(const Group *g) {
}

char *attrListToJSON(const List *list) {
}

char *circListToJSON(const List *list) {
}

char *rectListToJSON(const List *list) {
}

char *pathListToJSON(const List *list) {
}

char *groupListToJSON(const List *list) {
}

char *SVGtoJSON(const SVG *img) {
}

// MODULE 3 BONUS

SVG *JSONtoSVG(const char *svgString) {
}

Rectangle *JSONtoRect(const char *svgString) {
}

Circle *JSONtoCircle(const char *svgString) {
}
