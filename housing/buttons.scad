$fn = 36;

use <usbshare_elements.scad>;
include <usbshare_settings.scad>;

button_outerheight = 1.5;
button_innerheight = 3 - 1.2 - overprint;

echo(button_innerheight);

hodl_width = 0.65;

module buttons()
{
    for(pos = button_holes)
    {
        translate([pos.x, 0, 0])
        {
            cylinder(box_wall_thickness + button_outerheight, d=5);
            translate([0, 0, box_wall_thickness + button_outerheight])
            {
                cylinder(button_innerheight, d=10);
            }
        }
        
    }
    translate([(button_holes[1].x + button_holes[0].x) / 2, 4, box_wall_thickness + button_outerheight + button_innerheight - hodl_width / 2])
    {
        cube([abs(button_holes[1].x - button_holes[0].x), 2, hodl_width], center=true);
    }

}

buttons();