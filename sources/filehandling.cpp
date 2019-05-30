#include "filehandling.h"

#ifndef _WIN32
// make (sub)directories including multiple subdirs
int mkdirp(const char *fqfn, mode_t mode) {
  char *t, str[STRLEN];
  struct stat stbuf;
  int len;

  t=(char *)fqfn;
  memset(str, '\0', STRLEN);

  while (*t) {
    if (*t == '/') {
      len = t-fqfn;
      if ((len < STRLEN) && (len > 0)) {
        strncpy(str, fqfn, len);
        if (stat(str, &stbuf) != 0)
          if (mkdir(str, mode) != 0) return(-1);
      }
    }
    t++;
  }

  return(0);
}
#endif

// new fopen()
FILE *f1open(const char *f, const char *m, const enum filetype t) {
// #ifdef defined(_WIN32) || defined(__EMSCRIPTEN__)
//   // nothing changed here, works perfectly :)
//   printf("opening : %s\n", f);
//   return(fopen(f, m));
// #else
//   // *nix is a bitch ;)
//   printf("opening : %s\n", f);
//   char fname[STRLEN];

//   switch (t) {
//     case GAMEDATA:
      // gamedata is read-only
      return(fopen(f, m));
//       break;

//     case USERDATA:
//       // userdata is put in $HOME/.GAMENAME/
//       snprintf(fname, STRLEN-1, "%s/.%s/%s", getenv("HOME"), GAMENAME, f);
//       // create subdirs if they don't exist
//       mkdirp(fname, S_IRWXU | S_IRGRP|S_IXGRP | S_IROTH|S_IXOTH);
//       // open file
//       return(fopen(fname, m));
//       break;
//   }
// #endif

  // should not be reached
  //return(NULL);
}

