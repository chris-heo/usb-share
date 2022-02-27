$fn = 36;

use <usbshare_elements.scad>;
include <usbshare_settings.scad>;



module top_shell(box_size, wall_thickness, top_thickness)
{
    hodl_width = 1;
    hodl_height = 1;
    hodl_clearance = 3;
    
    hodl_offset_z = -(top_thickness + hodl_height)/2;
    front_height = box_size.z;
    verthodl_height = front_height - top_thickness - box_bottom_thickness - hodl_clearance;
    
    union() 
    {
        cube([box_size.x, box_size.y, top_thickness], true);
        
        // left hodl        
        color("red")
        translate([-(box_size.x - hodl_width) / 2 + (wall_thickness + overprint * 2), -(hodl_clearance + overprint) / 2, hodl_offset_z])
            cube([hodl_width, box_size.y - (2 * wall_thickness + hodl_clearance) + overprint, hodl_height], center = true);
        
        // right hodl
        translate([(box_size.x - hodl_width) / 2 - (wall_thickness + overprint * 2), -(hodl_clearance + overprint) / 2, hodl_offset_z])
            cube([hodl_width, box_size.y - (2 * wall_thickness + hodl_clearance) + overprint, hodl_height], center = true);
        
        // back hodl
        translate([0, (box_size.y - hodl_width) / 2 - (wall_thickness + overprint * 2), hodl_offset_z])
            cube([box_size.x - 2 * (wall_thickness + hodl_clearance), hodl_width, hodl_height], center = true);
        
        //front
        translate([0, -(box_size.y - wall_thickness) / 2 - overprint, -(front_height)/2])
            cube([box_size.x, wall_thickness, front_height + top_thickness], center = true);
        
        // front left hodl
        translate([
            -(box_size.x - hodl_width) / 2 + (wall_thickness + overprint * 2), 
            -(box_size.y - hodl_height) / 2 + wall_thickness - overprint,
            -(top_thickness + verthodl_height) / 2 - hodl_height
        ])
            cube([hodl_width, hodl_height, verthodl_height], center = true);
        
        // front right hodl
        translate([
            (box_size.x - hodl_width) / 2 - (wall_thickness + overprint * 2), 
            -(box_size.y - hodl_height) / 2 +wall_thickness - overprint,
            -(top_thickness + verthodl_height) / 2 - hodl_height
        ])
            cube([hodl_width, hodl_height, verthodl_height], center = true);
            
        color("red")
        
        translate([
            (box_size.x) / 2 - (wall_thickness + overprint * 2) - hodl_width, 
            -(box_size.y) / 2 +wall_thickness - overprint + 1,
            -1.5
        ])
        rotate([0,90, 0])
        {
            linear_extrude(hodl_width)
            polygon(points=[[0, 0], [10, 0], [0, 10]]);
        }
        
        
        translate([
            -(box_size.x) / 2 + (wall_thickness + overprint * 2), 
            -(box_size.y) / 2 +wall_thickness - overprint + 1,
            -1.5
        ])
        rotate([0,90, 0])
        {
            linear_extrude(hodl_width)
            polygon(points=[[0, 0], [10, 0], [0, 10]]);
        }
            
        // front reinforcement (shitty)
        translate([
            0, 
            -(box_size.y - hodl_height) / 2 + wall_thickness - overprint,
            
            -(top_thickness + front_height - box_bottom_thickness) / 2
        ])
            cube([5, hodl_height, front_height - box_bottom_thickness], center = true);
    }
}

module cap_body()
{
        translate([box_offset.x, box_offset.y, box_top_thickness/2])
            top_shell(box_size, box_wall_thickness, box_top_thickness);
}



difference()
{
    translate([0,0, box_size.z - box_bottom_thickness])
    {
        union()
        {
            cap_body();
            cap_domes();
        }
    }
   
    screwholes();
    _led_holes();
    _button_holes();
    
}



module screwholes()
{
    screw_length = 12;
    screw_headheight = 1.3;
    screw_inset = 0.25; // don't let the screw stick out
    color("#000080")
    translate([0,0,-box_bottom_thickness + screw_length + screw_headheight + screw_inset])
    {
        for(pos = pcb_holes)
        {
            translate([pos.x, pos.y, 0] - [pcb_size.x, pcb_size.y, 0] / 2)
            rotate([0, 180, 0])
            {
                //cylinder(pcb_compheight_top, d=studdiam);
                //flathead_screwhole(headdiam, headthickness, diam, length)
                flathead_screwhole(screwhead_diam + overprint * 2, screw_headheight, 2+0.2 + overprint * 2, screw_length); //M2 w/o overprint
                translate([0,0,-10])
                cylinder(10, d=screwhead_diam + overprint * 2);
            }
        }
    }       
}

module _led_holes()
{
    //translate([0,0,-box_bottom_thickness])
    {
        for(pos = led_holes)
        {
            translate(pos - [pcb_size.x, pcb_size.y, 0] / 2 + [0,0,pcb_size.z + pcb_compheight_bot])
            rotate([90, 0, 0])
            {
                cylinder(10, d=1.5);
            }
        }
    }
}

module _button_holes()
{
    //translate([0,0,-box_bottom_thickness])
    {
        for(pos = button_holes)
        {
            translate(pos - [pcb_size.x, pcb_size.y, 0] / 2 + [0,0,pcb_size.z + pcb_compheight_bot])
            rotate([90, 0, 0])
            {
                cylinder(15, d=5.5);
            }
        }
    }
}



module cap_domes()
{
    height_comp = 0.3;
    
    color("#00800080")
    {
        for(pos = pcb_holes)
        {
            translate([pos.x, pos.y, -pcb_compheight_top + height_comp] - [pcb_size.x, pcb_size.y, 0] / 2)
            {
                cylinder(pcb_compheight_top, d=studdiam);
            }
            
            //2nd set of support
            foo = 4;
            adddiam = 3;
            translate([pos.x, pos.y, -foo] - [pcb_size.x, pcb_size.y, 0] / 2)
            {
                cylinder(foo, d=screwhead_diam+adddiam);
                
            }
            translate([pos.x, pos.y, -foo-adddiam/2] - [pcb_size.x, pcb_size.y, 0] / 2)
                cylinder(adddiam/2, d2=screwhead_diam+adddiam, d1=screwhead_diam);
        }
    }
}



