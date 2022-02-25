//
// Created by David Pearson on 2022-01-17.
//

#ifndef A1_SVGHELPERS_H
#define A1_SVGHELPERS_H

#include "SVGParser.h"
#include <stdio.h>

#define PI 3.141592653589
#include <math.h>


void parser(xmlNode *a_node, SVG *svg);

void initSVG(SVG * svgReturn);
void fillSVG(SVG * svg, xmlNode * cur_node);

Rectangle* createRect(xmlNode * cur_node);

void initRect(Rectangle *rect);

Circle* createCircle(xmlNode * cur_node);

void initCircle(Circle *circle);

void createGroup(Group *g, xmlNode *cur_node);

void initGroup(Group *group);

Attribute *createAttr(char *name, char value[]);

Path *createPath(xmlNode * cur_node);

char *getNS(xmlNode *cur_node);

void setSVGDesc(SVG * svg, xmlNode * cur_node);
void setSVGTitle(SVG * svg, xmlNode * cur_node);

void dummyDel(void *data);

void findRect(Group *group, List *lst);
void findCirc(Group *group, List *lst);
void findGroup(Group *group, List *lst);
void findPaths(Group *group, List *lst);

//A2
int validateTree(char *fileName, char *xsdRef);

#endif //A1_SVGHELPERS_H
