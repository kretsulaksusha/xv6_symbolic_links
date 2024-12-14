//#include "types.h"
//#include "stat.h"
//#include "user.h"
//
//int
//main(int argc, char *argv[])
//{
//  if(argc != 3){
//    printf(2, "Usage: ln old new\n");
//    exit();
//  }
//  if(link(argv[1], argv[2]) < 0)
//    printf(2, "link %s %s: failed\n", argv[1], argv[2]);
//  exit();
//}

#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int (*ln)(const char*, const char*);

  ln = link;
  if(argc > 1 && strcmp(argv[1], "-s") == 0){
    ln = symlink;
    argc--;
    argv++;
  }

  if(argc != 3){
    printf(2, "Usage: ln [-s] old new (%d)\n", argc);
    exit();
  }
  if(ln(argv[1], argv[2]) < 0){
    printf(2, "%s failed\n", ln == symlink ? "symlink" : "link");
    exit();
  }
  exit();
}
