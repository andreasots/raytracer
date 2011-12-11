import bpy
obj = "Mesh"
f = open(obj+".txt", "w")

for face in bpy.data.objects[obj].data.faces:
    f.write("triangle \""+obj+"_mat\" ")
    for vert in face.vertices:        
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.x)+" ")
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.y)+" ")
        f.write(str(bpy.data.objects[obj].data.vertices[vert].co.z)+" ")
    f.write("flat\n")

f.close()
