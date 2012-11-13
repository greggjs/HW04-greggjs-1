#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "greggjsStarbucks.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    Surface* my_surface_;
    static const int kTextureSize=1024;
    
    greggjsStarbucks* starbucks;
    Entry* imported_values;
    int import_size;
    Entry* slow_sln;
    Entry* fast_sln;
    double x, y;
};

void HW04App::setup()
{
    my_surface_ = new Surface(kTextureSize, kTextureSize, false);
    
    imported_values = new Entry[10000];
    
    starbucks = new greggjsStarbucks();
    
    string filename = "../Starbucks_2006.csv";
    import_size = starbucks->importData(imported_values, filename);
    
    starbucks->build(imported_values, import_size);
    
    slow_sln = new Entry;
    fast_sln = new Entry;
    
    slow_sln = starbucks->alwaysRightValue(imported_values, import_size, x, y);
    fast_sln = starbucks->getNearest(x, y);
    
    console() << "Slow solution: " << endl << "City: " << slow_sln->identifier << endl
    << "x: " << slow_sln->x << endl << "y: " << slow_sln->y << endl;
	console() << "Fast solution: " << endl << "City: " << fast_sln->identifier << endl
    << "x: " << fast_sln->x << endl << "y: " << fast_sln->y << endl;
}

void HW04App::mouseDown( MouseEvent event )
{
}

void HW04App::update()
{
}

void HW04App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HW04App, RendererGl )
