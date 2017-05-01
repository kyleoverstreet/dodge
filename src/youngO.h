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
extern void moveRandomly(Player *player, Player *player2);
extern bool start_text_timer();
extern bool start_notext_timer();
extern bool check_text_timer(bool txt);
extern bool check_notext_timer(bool txt);

#endif
