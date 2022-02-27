$fn = 36;

use <usbshare_elements.scad>;
include <usbshare_settings.scad>;

use <usbshare_top.scad>;

// ground plate
//color("#80808080")

module bottom_shell(size, wall_thickness, bottom_thickness)
{
    bottom_overreach = 0.1;
    difference()
    {
        cube(size, true);
        translate([0,0, (bottom_thickness + bottom_overreach) / 2])
            cube(size - [wall_thickness * 2, wall_thickness * 2, bottom_thickness - bottom_overreach], true);
    }
}




// reference layer is the inner center of the box
module box_body()
{
    union()
    {
        translate(box_offset)
            bottom_shell(box_size, box_wall_thickness, box_bottom_thickness);

        //studs
        color("#800000")
        {
            for(pos = pcb_holes)
            {
                translate([pos.x, pos.y, 0] - [pcb_size.x, pcb_size.y, 0] / 2)
                {
                    cylinder(pcb_compheight_bot, d1=studdiam *1.5, d2=studdiam);
                }
            }
        }
    }
}

module box_screws_bot()
{
    // screw holes
    color("#000080")
    {
        for(pos = pcb_holes)
        {
            translate([pos.x, pos.y, -box_bottom_thickness] - [pcb_size.x, pcb_size.y, 0] / 2)
            {
                flathead_screwhole(3.7+0.3, 1.3+0.2, 2+0.2, 12); //M2 w/o overprint
            }
        }
    }
}

module box_screws()
{
    // screw holes
    color("#000080")
    {
        for(pos = pcb_holes)
        {
            translate([pos.x, pos.y, -box_bottom_thickness] - [pcb_size.x, pcb_size.y, 0] / 2)
            {
                //flathead_screwhole(3.7+0.3, 1.3+0.2, 2+0.2, 12); //M2 w/o overprint
                hex_screwhole(4.5+0.5, 2.0+0.2, 2+0.2, 12);
            }
        }
    }
}

intersection()
{
 
    difference()
    {
        color(alpha=0.5)
        box_body();
        box_screws();
        usb_back();
        usb_side();
        //no front
        translate([0, box_offset.y -box_size.y/2 + box_wall_thickness / 2, -box_bottom_thickness + box_size.z/2])
        cube([box_size.x, box_wall_thickness, box_size.z]+[0.0002,0.0002,0.0002], center = true);
    }
    
    //translate([0, -18.5, 0])
    //    cube([55, 15, 25], center=true);
}


if(false)
{
    // show the PCB (just for debugging)
    translate([0,0, pcb_compheight_bot + pcb_size.z / 2])
        board(pcb_size, pcb_holes);
}

module usb_back()
{
    usba_coord = [25, 27.1475, 0];
    usba_length = 13.7+2;

    translate(usba_coord + [-pcb_size.x / 2, -pcb_size.y / 2, pcb_compheight_bot + pcb_size.z + 0.2]) 
    rotate([90, 0, 180])
    linear_extrude(usba_length + box_wall_thickness)
    usba_cutout();


    usbmb_coords = [
        [10, 38.41875, 0],
        [40, 38.41875, 0],
    ];
    for(pos = usbmb_coords)
    {   
        translate(pos + [-pcb_size.x / 2, -pcb_size.y / 2, pcb_compheight_bot + pcb_size.z + 0.2]) 
        rotate([90, 0, 180])
        linear_extrude(5 + box_wall_thickness)
        microusb_cutout();
    }
}

module usb_side()
{
    pos = [1.5875, 15.79, 0];
    translate(pos + [-pcb_size.x / 2, -pcb_size.y / 2, pcb_compheight_bot + pcb_size.z + 0.2]) 
    rotate([90, 0, 180+90])
    linear_extrude(5 + box_wall_thickness)
    microusb_cutout();
}
//translate([0,0, box_bottom_thickness + pcb_compheight_bot])
//    board();

//countersunk_screwhole(5, 2.2, 5);





//