#include "kyleO.cpp"
#include "koryD.cpp"
#include "jacobW.cpp"
#include "youngO.cpp"

extern void printChristian();
extern void upload_socres(const char*);


int main()
{
    //Kyle's function - uploads scores to webpage
    const char *file_path = "/home/stu/koverstreet/public_html/3350/dodge.html";
    upload_scores(file_path);
    
    printChristian();
    jacob();
    kory();
    youngsoo();
    return 0;
}
