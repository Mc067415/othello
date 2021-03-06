#ifndef PIECE_H
#define PIECE_H
#include "sphere.h"
#include <vector>
#include <string>
using std::string;

class Piece : public object{
    public:
        Piece(
            vector<point4>& globalpoints,
            vector<color4>& globalcolors,
            vector<norm4>& globalnormals, 
            GLint nmodel_view ,int nInitial_Point
        );
        void draw();
        
        void rotatetowhite();
        void settowhite();

    	void rotatetoblack();
		void settoblack();

        void translatetopostion(string othelloStrmove,bool placed);  
        void setposition(string othelloStrmove,bool placed);
        void reset();

        vec2 boardposition(){return posOnboard;}

    private:
        sphere* spheres[2];
        vec3 scalefactor = vec3(1,.4,1); // used to make the piece look kinda smashed

        vec2 othelloStrMvToPostition(string move);
        vec2 posOnboard = vec2(-1,-1);
};

#endif