/**
 * @file SVGHelpers.h
 * @author David Pearson (1050197)
 * @brief 
 * 
 */

#ifndef SVGHELPERS_H
#define SVGHELPERS_H

#include "SVGParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589

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
void rectToXML(xmlNodePtr pNode, Rectangle *rect);
void circToXML(xmlNodePtr pNode, Circle *circ);
void pathToXML(xmlNodePtr pNode, Path *path);
void groupToXML(xmlNodePtr pNode, Group *group);
xmlDocPtr svgToTree(const SVG *svg);
int validateTree(xmlDocPtr doc, const char *xsdRef);

int validateRect(Rectangle *rect);
int validateCirc(Circle *circ);
int validatePath(Path *path);
int validateGroup(Group *group);
int validateContents(SVG *svg);

Path *realloc_Path(Path *p, int size);

const char *fileEXT(const char *filename);
bool addOtherAttribute(List *otherAttr, Attribute *newAttr);
bool addRectAttr(List *rects, int index, Attribute *newAttr);
bool addCircAttr(List *circs, int index, Attribute *newAttr);
bool addPathAttr(List *paths, int index, Attribute *newAttr);
bool addGroupAttr(List *groups, int index, Attribute *newAttr);
#endif
