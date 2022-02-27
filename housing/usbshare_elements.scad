module microusb_cutout(overprint = 0)
{
    //AMP 10118194-0001LF
    width = 6.9;
    height = 1.85;
    chamfer = 0.7;
    metal_thickness = 0.25;
    fins_width = 0.325;
    
    translate([0, metal_thickness, 0])
    offset(r=(metal_thickness + fins_width + overprint))
    {
        polygon([
            [-width / 2, height],
            [width / 2, height],
            [width / 2, chamfer],
            [width / 2 - chamfer, 0],
            [-width / 2 + chamfer, 0],
            [-width / 2, chamfer],
        ]);
    }
}

module usba_cutout(overprint = 0)
{
    // Assmann A-USB A/SMT
    width = 14.5-1;
    height = 7.12;
    height_offset = 0.4;
    roundness = 1;
    
    translate([0, height_offset + height / 2, 0])

    offset(r = roundness)
    {
        square([width - roundness * 2, height - roundness * 2], true);
        circle(r=(roundness + overprint));
    }
}


module stud()
{
    color("#800000")
    {
        for(stud = pcb_holes)
        {
            translate([stud.x, stud.y, box_bottom_thickness])
            {
                cylinder(pcb_compheight_bot, d=studdiam);
            }
        }
    }
}

module board(size, holes)
{
    hole_overlap = 0.2;
    color("#00800080")
    {
        difference()
        {
            cube(size, true);
            for(hole = holes)
            {
                translate([hole.x - size.x / 2, hole.y - size.y / 2, 0])
                {
                    cylinder(size.z + hole_overlap, d=hole.z,center=true);
                }
            }
        }
        
    }
}

module countersunk_screwhole(headdiam, diameter, length)
{
    union()
    {
        cylinder(h=headdiam/2, d1=headdiam, d2=0);
        cylinder(h=length, d1=diameter);
    }
}

module flathead_screwhole(headdiam, headthickness, diam, length)
{
    union()
    {
        cylinder(headthickness, d = headdiam);
        cylinder(headthickness + length, d = diam);
    }
}

module hex_screwhole(head_outer_diam, head_thickness, diam, length)
{
    union()
    {
        //cylinder(d=4.5+0.3, h=2, $fn=6);
        cylinder(d=head_outer_diam, h=head_thickness, $fn=6);
        cylinder(length, d = diam);
    }
}

module akl101_wireholes(pins)
{
    pitch = 5.08;
    wire_hole_diam = 3.5;
    wire_height = 3.3;
    hole_len = 10;
    
    for(i = [1 : pins])
    {
        
        rotate([90, 0, 0])
        translate([pitch * (i - 1 - (pins - 1) / 2), wire_height, hole_len / 2])
        
        cylinder(hole_len, d=wire_hole_diam, center=true);
    }
}

module akl101_screwholes(pins)
{
    pitch = 5.08;
    screw_hole_diam = 3.5;
    hole_len = 10;
    
    for(i = [1 : pins])
    {
        
        translate([pitch * (i - 1 - (pins - 1) / 2), 0, hole_len / 2])
        
        cylinder(hole_len, d=screw_hole_diam, center=true);
    }
}
