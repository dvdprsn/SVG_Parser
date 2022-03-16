#include "SVGHelpers.h"
#include "SVGParser.h"

int main(int argc, char **argv) {
    // SVG creation, validation and write out
    // SVG *svg = createSVG(argv[1]);
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

    SVG *jsonSVG = JSONtoSVG("\"title\":\"stuffers in the powep\",\"descr\":\"junkdsaf sdf22 fsdf\"");
    char *svgTest = SVGToString(jsonSVG);
    printf("%s\n", svgTest);
    free(svgTest);
    deleteSVG(jsonSVG);

    Rectangle *jsonRect = JSONtoRect("{\"x\":2.53,\"y\":3.43,\"w\":12,\"h\":10,\"units\":\"\"}");
    char *rectStr = rectangleToString(jsonRect);
    printf("%s\n",rectStr);
    free(rectStr);
    deleteRectangle(jsonRect);

    Circle *jsonCirc = JSONtoCircle("{\"cx\":4.53,\"cy\":7.43,\"r\":20,\"units\":\"\"}");
    char *circStr = circleToString(jsonCirc);
    printf("%s\n", circStr);
    free(circStr);
    deleteCircle(jsonCirc);

    // Test setAttr
    // SVG attributes

    Attribute *attr3 = createAttr("width", "16.31cm");  // Should fail
    if (setAttribute(svg, RECT, 0, attr3) == true) {
    } else {
        deleteAttribute(attr3);
    }

    write = writeSVG(svg, "test_alt.svg");
    if (!write) {
        printf("Failed to write\n");
        deleteSVG(svg);
        return 0;
    }


    // JSON rects
    List *rects = getRects(svg);
    char *rectJSON = rectListToJSON(rects);
    printf("Rectangle: ");
    printf("%s\n", rectJSON);
    free(rectJSON);
    freeList(rects);

    // JSON circs
    List *circles = getCircles(svg);
    char *circs = circListToJSON(circles);
    printf("Circle: ");
    printf("%s\n", circs);
    free(circs);
    freeList(circles);

    // JSON paths
    List *paths = getPaths(svg);
    char *pathJSON = pathListToJSON(paths);
    printf("Path: ");
    printf("%s\n", pathJSON);
    free(pathJSON);
    freeList(paths);

    // JSON groups
    List *groups = getGroups(svg);
    char *groupJSON = groupListToJSON(groups);
    printf("Group: ");
    printf("%s\n", groupJSON);
    free(groupJSON);
    freeList(groups);

    // SVG JSON
    char *svgJSON = SVGtoJSON(svg);
    printf("SVG: ");
    printf("%s\n", svgJSON);
    free(svgJSON);

    deleteSVG(svg);

    printf("%s\n", wrapSVGtoJSON(argv[1], "svg.xsd"));

    return 0;
}
