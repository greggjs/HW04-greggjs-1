#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "greggjsStarbucks.h"
#include "CensusData.h"
#include "LinkList.h"
#include <vector>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04App : public AppBasic {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
	void update();
	void draw();
    void prepareImages();
    
    
    
    greggjsStarbucks* starbucks;
    Entry* imported_values;
    int import_size;
    ///Entry* slow_sln;
    ///Entry* fast_sln;
    double x, y;
    gl::Texture map_texture_, texture_;
    
    Vec2f top_left_, bottom_right_;
    
    bool enable_map_, enable_PS_, enable_area_;
    Entry* closest;
    
    LinkList* starbucks_list_;
    LinkList* census_list_;
    
};

static const int kTextureSize=512;

void HW04App::prepareSettings(Settings* settings) {
    (*settings).setWindowSize(kTextureSize, kTextureSize);
	(*settings).setResizable(false);
}

void HW04App::setup()
{
    enable_map_ = enable_PS_ = enable_area_ = false;
    closest = NULL;

    /// make starbucks tree
    imported_values = new Entry[10000];
    
    starbucks = new greggjsStarbucks();
    
    string filename = "../resources/Starbucks_2006.csv";
    import_size = starbucks->importData(imported_values, filename);
    
    starbucks->build(imported_values, import_size);
    
    top_left_ = Vec2f(0,0);
    bottom_right_ = Vec2f(1,1);
    
    starbucks_list_ = new LinkList();
    starbucks_list_->next_ = NULL;
    census_list_ = new LinkList();
    census_list_->next_ = NULL;
    
    prepareImages();
    
    /* Test Code
    slow_sln = new Entry;
    fast_sln = new Entry;
    
    slow_sln = starbucks->alwaysRightValue(imported_values, import_size, x, y);
    fast_sln = starbucks->getNearest(x, y);
    
    console() << "Slow solution: " << endl << "City: " << slow_sln->identifier << endl
    << "x: " << slow_sln->x << endl << "y: " << slow_sln->y << endl;
	console() << "Fast solution: " << endl << "City: " << fast_sln->identifier << endl
    << "x: " << fast_sln->x << endl << "y: " << fast_sln->y << endl;
     */
}

void HW04App::mouseDown( MouseEvent event )
{
    if (event.isRight()) {
        double mouse_x = (event.getX()*1.0)/getWindowWidth();
        double mouse_y = (event.getY()*1.0)/getWindowHeight();
        
        closest = starbucks->getNearest(mouse_x, mouse_y);
    }
}

void HW04App::keyDown(KeyEvent event) {
    if (event.getCode() == event.KEY_p) enable_PS_ = !enable_PS_;
    else if (event.getCode() == event.KEY_m) enable_map_ = !enable_map_;
    else if (event.getCode() == event.KEY_a) enable_area_ = !enable_area_;
}

void HW04App::prepareImages() {
    CensusData* census_2010 = new CensusData("../resources/Census_2010.csv");
    census_2010->buildCensusData(census_2010->file_location_);
    
    CensusData* census_2000 = new CensusData("../resources/Census_2000.csv");
    census_2000->buildCensusData(census_2000->file_location_);
    
	console() << "Generating Population Tiles" << endl;
    int w = getWindowWidth();
    int h = getWindowHeight();
    
    for (int x = 0; x < w; x+= 3) {
        for (int y = 0; y < h; y+= 3) {
            LinkList* temp = new LinkList();
            temp->next_ = census_list_->next_;
            census_list_->next_ = temp;
            
            CensusData* data_2010 = census_2010->getNearest((x+.0)/w, (y+.0)/h);
            CensusData* data_2000 = census_2000->getNearest((x+.0)/w, (y+.0)/h);
            Entry* closest_starbucks = starbucks->getNearest(x, y);
            
            temp->closest_starbucks_color_ = Color8u((int)(closest_starbucks->x*255),(int)((1-closest_starbucks->y)*255),(int)(closest_starbucks->x*closest_starbucks->y*255));
            temp->closest_starbucks_ = Vec2f(closest_starbucks->x, closest_starbucks->y);
            int population_difference = data_2010->population_ - data_2000->population_;
            
            if (population_difference > 500) temp->color_ = ColorA( 0, 150, 0, .1f);
            else if (population_difference < -500) temp->color_ = ColorA( 150, 0, 0, .1f);
            else temp->color_ = ColorA(150, 150, 0, .1f);
            
            temp->vec_ = Vec2f((x+.0)/w, (y+.0)/h);
            temp->rect_ = Rectf((x+.0)/w, (y+.0)/h, (x+2.0)/w, (y+2.0)/h);
        }
    }
    
	console() << "Finished with Census" << endl;

	/// Taken from https://github.com/harmonbc/HW04_harmonbc/blob/master/resources/squaremurica.bmp
	/// Thought this was a good image to use, since it was already scaled
    Surface map(loadImage("../resources/squaremurica.bmp"));
    map_texture_ = gl::Texture(map);
    
	starbucks->getArea(top_left_, bottom_right_, starbucks->search_root_, true, starbucks_list_);
}

void HW04App::update()
{
    gl::clear(Color(0, 0, 0));
    gl::color(255,255,255);
}

void HW04App::draw()
{
	int w = getWindowWidth();
    int h = getWindowHeight();
    
    LinkList* temp = census_list_->next_;
    gl::enableAlphaBlending();
    Vec2f current_position = getMousePos();
    
    if (enable_map_) gl::draw(map_texture_, getWindowBounds());
    while(temp!=NULL&&(enable_PS_||enable_area_)) {
        if (enable_PS_) {
            Rectf r = temp->rect_;
            if (temp->vec_.distance(Vec2f(current_position.x/w, current_position.y/h)) < .1)
                gl::color(ColorA(temp->color_.r, temp->color_.g, temp->color_.b, .25f));
            else gl::color(temp->color_);
            gl::drawSolidRoundedRect(Rectf(r.x1*w, r.y1*h, r.x2*w, r.y2*h), 0);
        }
        if (enable_area_) {
            gl::color(temp->closest_starbucks_color_);
            gl::drawLine(Vec2f((temp->vec_.x)*w, (temp->vec_.y)*h), Vec2f((temp->closest_starbucks_.x)*w, (temp->closest_starbucks_.y)*h));
            
        }
        temp = temp->next_;
    }
	
    LinkList* list_temp = starbucks_list_->next_;
    while(list_temp!=NULL) {
        gl::color(list_temp->color_);
        gl::drawSolidCircle(Vec2f((list_temp->vec_.x)*w, (list_temp->vec_.y)*h), 2);
        list_temp = list_temp->next_;
    }
    if (closest!=NULL)
        gl::drawSolidCircle(Vec2f(closest->x*w, closest->y*h), 5);
    
    Entry* temp_closest = starbucks->getNearest((current_position.x+.0)/w, (current_position.y+.0)/h);
    gl::drawLine(current_position, Vec2f(temp_closest->x*w, temp_closest->y*h));
}

CINDER_APP_BASIC( HW04App, RendererGl )
