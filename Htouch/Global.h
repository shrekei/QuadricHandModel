#pragma once

//#define HTOUCH_DEBUG

// USER MESSAGE DEFINITION
#define WM_USER_REPAINT WM_USER+5

// commonly used quantities
const double RADTODEG = 57.2958; // 360 / (2 * PI)
const double INF = 1.79e+308;
const double PI = 3.1415927;

// files in system directory
const char g_strHandCascade[] = "../ProjResource/handclassifier.xml";
const char g_strHandColorTemplate[] = "../ProjResource/template.jpg";