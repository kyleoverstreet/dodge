#ifndef YOUNGO_H
#define YOUNGO_H

extern void createSpikes(const int n, const int xres, const int yres);
extern void createHelmets(const int n, const int xres, const int yres);
extern void createStars(const int n, const int xres, const int yres);
extern void createHeart(const int n, const int xres, const int yres);
extern void drawSpikes(void);
extern void drawHelmets(void);
extern void drawStars(void);
extern void drawHeart(void);
extern void deleteHelmet(Helmet *node);
#endif
