#include "SVGHelpers.h"
#include "SVGParser.h"

int main(int argc, char **argv) {

    //SVG creation, validation and write out
    //SVG *svg = createSVG(argv[1]);
    SVG *svg = createValidSVG(argv[1], "svg.xsd");
    if (svg == NULL) {
        return 0;
    }

    bool valid = validateSVG(svg, "svg.xsd");
    if (!valid) {
        printf("Invalid catch\n");
        deleteSVG(svg);
        return 0;
    }

    bool write = writeSVG(svg, "test.svg");
    if (!write) {
        printf("Failed to write\n");
        deleteSVG(svg);
        return 0;
    }
    printf("Attr: %d\n", numAttr(svg));
    //Test setAttr
    //SVG attributes
    Attribute *attr = createAttr("viewBox", "0 0 1200 600"); //Should work
    if(setAttribute(svg, SVG_IMG, 0, attr) == true) {
    } else {
        printf("failed\n");
        deleteAttribute(attr);
    }
    Attribute *attr2 = createAttr("height", "13.64cm"); //Should work
    if(setAttribute(svg, SVG_IMG, 0, attr2) == true) {
    } else {
        printf("failed\n");
        deleteAttribute(attr2);
    }
    
    //Rectangle Attributes
    Attribute *attr3 = createAttr("width", "13.64cm"); //Should fail
    if(setAttribute(svg, RECT, 0, attr3) == true) {
    } else {
        printf("failed\n");
        deleteAttribute(attr3);
    }
    
    Attribute *attr5 = createAttr("d", "test");
    if(setAttribute(svg, PATH, 0, attr5) == true){
    } else {
        printf("attr path failed to add\n");
        deleteAttribute(attr5);
    }

    write = writeSVG(svg, "test_alt.svg");
    if (!write) {
        printf("Failed to write\n");
        deleteSVG(svg);
        return 0;
    }
    printf("Attr: %d\n", numAttr(svg));

    //JSON rects
    List *rects = getRects(svg);
    char *rectJSON = rectListToJSON(rects);
    printf("%s\n",rectJSON);
    free(rectJSON);
    freeList(rects);

    //JSON circs
    List *circles = getCircles(svg);
    char *circs = circListToJSON(circles);
    printf("%s\n", circs);
    free(circs);
    freeList(circles);

    //JSON paths
    List *paths = getPaths(svg);
    char *pathJSON = pathListToJSON(paths);
    printf("%s\n",pathJSON);
    free(pathJSON);
    freeList(paths);

    //JSON groups
    List *groups = getGroups(svg);
    char *groupJSON = groupListToJSON(groups);
    printf("%s\n", groupJSON);
    free(groupJSON);
    freeList(groups);

    //SVG JSON
    char *svgJSON = SVGtoJSON(svg);
    printf("%s\n", svgJSON);
    free(svgJSON);

    deleteSVG(svg);

    return 0;
}
