#ifndef YOUNGO_H
#define YOUNGO_H

extern void createItems(const int n, const int xres, const int yres);
extern void createHelmets(const int n, const int xres, const int yres);
extern void drawItems(void);
extern void drawHelmets(void);
extern void deleteHelmet(Helmet *node);

#endif
