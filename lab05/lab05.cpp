#include <iostream>
#include <cstdlib>
using namespace std;

typedef struct Gallery {
    int** map;
    int lines;
    int columns;
} Gallery;

typedef struct Instancies {
    Gallery* galleries;
    int size;
} Instancies;

Instancies readGalleries();
void print(Instancies instancies);

int main() {
    Instancies instancies = readGalleries();
    print(instancies);
    return 0;
}

Instancies readGalleries() {
    Instancies instancies;
    cin >> instancies.size;

    instancies.galleries = (Gallery*) malloc(instancies.size * sizeof(Gallery));

    for(int i = 0; i < instancies.size; i++) {
        int lines, columns;
        cin >> lines >> columns;

        instancies.galleries[i].lines = lines;
        instancies.galleries[i].columns = columns;
        
        int** gallery = (int**) malloc(lines * sizeof(int*));
        for(int line = 0; line < lines; line++) {
            gallery[line] = (int*) malloc(columns * sizeof(int));

            string galleryLine;
            cin >> galleryLine;
            
            for(int k = 0; k < columns; k++) {
                char sectorType = galleryLine[k];
                if(sectorType == '#') {
                    gallery[line][k] = 1;
                } else {
                    gallery[line][k] = 0;
                }
            }
        }
        instancies.galleries[i].map = gallery;
    }

    return instancies;
}

void print(Instancies instancies) {
    cout << instancies.size << "\n";
    for(int i = 0; i < instancies.size; i++) {
        Gallery gallery = instancies.galleries[i];
        cout << gallery.lines << " " << gallery.columns << "\n";

        for(int i = 0; i < gallery.lines; i++) {
            for(int k = 0; k < gallery.columns; k++) {
                if(gallery.map[i][k] == 1) {
                    cout << "#";
                } else if(gallery.map[i][k] == 0) {
                    cout << ".";
                }
            }
            cout << "\n";
        }
    }
}