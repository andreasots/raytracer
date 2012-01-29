import bpy
obj = "Plane.001"
f = open(obj+".txt", "w")

mesh = bpy.data.objects[obj].to_mesh(bpy.context.scene, True, 'RENDER')

for face in mesh.faces:
    if len(face.vertices) == 3:
        f.write("triangle")
    elif len(face.vertices) == 4:
        f.write("quad")
    else:
        raise ""
    f.write(" \""+obj+"\" ")
    for vert in face.vertices:
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.x)+" ")
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.y)+" ")
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.z)+" ")
    f.write("flat\n")

f.close()
