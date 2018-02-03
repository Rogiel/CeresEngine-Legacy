import bpy, os, sys, json, collections
import argparse

argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"
resources_out_dir = argv[0]

is_cycles = (bpy.context.scene.render.engine == 'CYCLES')

bpy.context.scene.cycles.samples = 1
bpy.context.scene.cycles.preview_samples = 1

scene = bpy.context.scene
for obj in scene.objects:
    obj.select = False

outdir = scene.name

image = bpy.data.images.new("Baking", width=1024, height=1024)
bpy.data.screens['UV Editing'].areas[1].spaces[0].image = image

scene_descriptor = collections.OrderedDict(
    name=bpy.context.scene.name, 
    objects=[])

for obj in scene.objects:
    if obj.type != 'MESH':
        continue

    # Hidden objects are not included.
    if obj.hide_render:
        continue

    scene.objects.active = obj
    obj.select = True

    uv_name = 0
    keep_subsurf_modifiers = True
    if 'xyz_engine' in obj:
        props = obj['xyz_engine']
        if 'keep_subsurf' in props:
            keep_subsurf_modifiers = props['keep_subsurf']
        if 'bake_uv' in props:
            uv_name = props['bake_uv']

    if len(obj.data.uv_textures) == 0:
        print(obj.name + ' has no UV maps! Creating one now...')
        bpy.ops.mesh.uv_texture_add()
        bpy.ops.object.editmode_toggle()
        bpy.ops.uv.smart_project(angle_limit=66)
        uv_name = 0

    # bake textures
    uv = obj.data.uv_textures[uv_name]
    uv.active = True

    print(" -- Exporting object", obj.name)

    # Specify the bake type
    bpy.context.scene.render.bake_type = "TEXTURE"

    # create a new folder for this object
    object_dir = os.path.join(outdir, obj.name)
    object_dir_abs = os.path.join(resources_out_dir, object_dir)
    if not os.path.exists(object_dir_abs):
        os.makedirs(object_dir_abs)

    # only bake textures if a material is present
    diffuse_path = None
    specular_path = None
    normal_path = None
    ao_path = None
    if obj.data.materials:
        for material in obj.data.materials:
            print(" -- Material " + material.name)

            # deselect any selected node
            for node in material.node_tree.nodes:
                node.select = False

            node = material.node_tree.nodes.new("ShaderNodeTexImage")
            node.image = image
            node.label = 'baked_texture'
            node.name = 'baked_texture'

            # set node as selected & active
            node.select = True
            material.node_tree.nodes.active = node

        scene.cycles.preview_samples = scene.cycles.samples = 1

        diffuse_path = os.path.join(object_dir, obj.name + '_diffuse.png')
        bpy.ops.object.bake(
            type='DIFFUSE', pass_filter={'COLOR'},
            width=image.generated_width, height=image.generated_height,
            margin=1, use_clear=True
        )
        image.file_format = 'PNG'
        image.filepath_raw = os.path.join(resources_out_dir, diffuse_path)
        image.save()

        specular_path = os.path.join(object_dir, obj.name + '_specular.png')
        bpy.ops.object.bake(
            type='GLOSSY', pass_filter={'COLOR'},
            width=image.generated_width, height=image.generated_height,
            margin=1, use_clear=True
        )
        image.file_format = 'PNG'
        image.filepath_raw = os.path.join(resources_out_dir, specular_path)
        image.save()

        # On Normals, we require more samples
        # bpy.context.scene.cycles.samples = 2
        # bpy.context.scene.cycles.preview_samples = 2

        normal_path = os.path.join(object_dir, obj.name + '_normal.png')
        bpy.ops.object.bake(
            type='NORMAL',
            normal_space='TANGENT',
            width=image.generated_width, height=image.generated_height,
            margin=1, use_clear=True
        )
        image.file_format = 'PNG'
        image.filepath_raw = os.path.join(resources_out_dir, normal_path)
        image.save()

        # these maps require a bit more quality to be rendered...
        scene.cycles.preview_samples = scene.cycles.samples = 256

        ao_path = os.path.join(object_dir, obj.name + '_ao.png')
        # bpy.ops.object.bake(
        #     type='AO',
        #     width=image.generated_width, height=image.generated_height,
        #     margin=1, use_clear=True
        # )
        # image.file_format = 'PNG'
        # image.filepath_raw = os.path.join(resources_out_dir, ao_path)
        # image.save()

    # apply all modifiers
    for modifier in obj.modifiers:
        if modifier.type == 'SUBSURF' and keep_subsurf_modifiers:
            continue
        print(" -- Applying modifier", modifier.name)
        bpy.ops.object.modifier_apply(modifier=modifier.name)

    # apply scale, rotation, etc..
    bpy.ops.object.transform_apply(
        location=True,
        scale=True,
        rotation=True)

    # re-pivot it around its geometric center
    bpy.ops.object.origin_set(type='ORIGIN_GEOMETRY')

    location = obj.location.copy()
    obj.location = (0, 0, 0)

    mesh_file = os.path.join(object_dir, obj.name + '.obj')
    bpy.ops.export_scene.obj(
        filepath=os.path.join(resources_out_dir, mesh_file),
        check_existing=True,
        axis_forward='-Z',
        axis_up='Y',
        use_selection=True,
        use_animation=False,
        use_mesh_modifiers=True,
        use_edges=False,
        use_smooth_groups=False,
        use_smooth_groups_bitflags=False,
        use_normals=True,
        use_uvs=True,
        use_materials=False,
        use_triangles=True,
        use_nurbs=False,
        use_vertex_groups=False,
        use_blen_objects=False,
        group_by_object=False,
        group_by_material=False,
        keep_vertex_order=False,
        global_scale=1.0,
        path_mode='AUTO'
    )

    scene_descriptor['objects'].append(collections.OrderedDict(
        name=obj.name, mesh=mesh_file,
        material=collections.OrderedDict(
            type='pbr',
            albedo=diffuse_path,
            metallic=specular_path,
            roughness=None,
            normal=normal_path,
            ambientOcclusion=ao_path),
        position=[
            location[0],
            location[1],
            location[2]],
        rotation=[
            obj.rotation_euler[0],
            obj.rotation_euler[1],
            obj.rotation_euler[2]],
        scale=[
            obj.scale[0],
            obj.scale[1],
            obj.scale[2]]))

    # restore object original state
    obj.select = False
    obj.location = location

# create a new folder for this object
scene_file = os.path.join(resources_out_dir, scene.name + '.json')
with open(scene_file, 'w') as f:
    json.dump(scene_descriptor, f, indent=2)
