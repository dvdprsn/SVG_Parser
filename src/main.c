#include "SVGParser.h"
#include "SVGHelpers.h"

int main(int argc, char **argv) {

    SVG *svg = createSVG(argv[1]);
    if(svg == NULL){
        printf("Failed to create SVG\n");
    }
    
    /**
     * Current issue is that with SVGtoString running the height attribute prints as NULL in the testing print in deleteAttribute 
     * after the struct passes through toString
     * Once we go to deleteSVG it claims I am freeing memory that isnt there
     * However, if SVGToString is commented out everything APPEARS to run as intended
     * 
     */
    //TODO Ask about Branch
    //TODO Ask about nested groups
    char *string = SVGToString(svg);

    printf("%s\n", string);

    deleteSVG(svg); 
    //free(string); //TODO free
    return 0;
}
