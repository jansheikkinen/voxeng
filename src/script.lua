i = 10

function draw()
    for x=-i,i do
        for y=-i,i do
            for z=-i,i do
                setVoxel(x, y, z)
            end
        end
    end
end
