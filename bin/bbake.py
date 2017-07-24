import bpy, os, sys
import argparse

argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"
outdir = argv[0]

is_cycles = (bpy.context.scene.render.engine == 'CYCLES')

bpy.context.scene.cycles.samples = 1
bpy.context.scene.cycles.preview_samples = 1

scene = bpy.context.scene
for obj in scene.objects:
    obj.select = False

image = bpy.data.images.new("Baking", width=1024, height=1024)
bpy.data.screens['UV Editing'].areas[1].spaces[0].image = image

for obj in scene.objects:
    if obj.type != 'MESH':
        continue

    # Hidden objects are not included.
    if obj.hide_render:
        continue

    scene.objects.active = obj
    obj.select = True

    uv_name = 0
    remove_subsurf_modifiers = True
    if 'xyz_engine' in obj:
        props = obj['xyz_engine']
        if 'keep_subsurf' in props:
            remove_subsurf_modifiers = (props['keep_subsurf'] == 0)
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

    # Specify the bake type
    bpy.data.scenes["Scene"].render.bake_type = "TEXTURE"

    for material in obj.data.materials:
        print(" -- Material " + material.name)

        if 'BakingTexture' not in bpy.data.textures:
            bpy.data.textures.new("BakingTexture", 'IMAGE')
        texture = bpy.data.textures['BakingTexture']

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

    object_dir = os.path.join(outdir, 'Objects', obj.name)
    if not os.path.exists(object_dir):
        os.makedirs(object_dir)

    bpy.ops.object.bake(
        type='DIFFUSE', pass_filter={'COLOR'},
        width=image.generated_width, height=image.generated_height,
        margin=1, use_clear=True
    )
    image.file_format = 'PNG'
    image.filepath_raw = os.path.join(object_dir, obj.name + '_diffuse.png')
    image.save()

    bpy.ops.object.bake(
        type='GLOSSY', pass_filter={'COLOR'},
        width=image.generated_width, height=image.generated_height,
        margin=1, use_clear=True
    )
    image.file_format = 'PNG'
    image.filepath_raw = os.path.join(object_dir, obj.name + '_specular.png')
    image.save()

    # On Normals, we require more samples
    # bpy.context.scene.cycles.samples = 2
    # bpy.context.scene.cycles.preview_samples = 2

    bpy.ops.object.bake(
        type='NORMAL', #normal_space='OBJECT',
        normal_space='TANGENT',
        width=image.generated_width, height=image.generated_height,
        margin=1, use_clear=True
    )
    image.file_format = 'PNG'
    image.filepath_raw = os.path.join(object_dir, obj.name + '_normal.png')
    image.save()

    # Now, remove subsurf modifiers
    if remove_subsurf_modifiers:
        for modifier in obj.modifiers:
            if modifier.type != 'SUBSURF':
                continue
            obj.modifiers.remove(modifier)

    bpy.ops.export_scene.obj(
        filepath=os.path.join(object_dir, obj.name + '.obj'),
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

    # bpy.ops.object.bake(type='COMBINED')
    obj.select = False
