#ifndef cube_door_h
#define cube_door_h

#include <vector>
#include "cube.h"
using std::vector;

class cube_door: public cube
{
public:
  cube_door(
    vector<point4>& globalpoints,
    vector<color4>& globalcolors,
    vector<norm4>& globalnormals,
    vector<color4>& wallcolors,
    GLint shader_mv_loc,
    int startinvao
  );
  void draw();
  void set_doorAngle(GLfloat ndoorAngle){
    doorAngle=ndoorAngle;
  };

private:
  GLfloat doorAngle;

};
#endif