import bpy
from bpy.props import *
import bpy.types


class XYZObjectPanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "XYZ Engine"
    bl_idname = "OBJECT_PT_xyz_engine"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    def draw(self, context):
        layout = self.layout

        obj = context.object  # type: bpy.types.Object

        row = layout.row()
        row.prop(obj.xyz_engine, 'keep_subsurf', text='Keep Subsurface modifier when baking')

        row = layout.row()
        row.prop_search(obj.xyz_engine, 'bake_uv', text='Baking UV', search_data=obj.data,
                        search_property='uv_textures')


class XYZObjectSettings(bpy.types.PropertyGroup):
    keep_subsurf = bpy.props.BoolProperty()
    bake_uv = bpy.props.StringProperty()


def register():
    bpy.utils.register_class(XYZObjectSettings)

    bpy.types.Object.xyz_engine = bpy.props.PointerProperty(type=XYZObjectSettings)

    bpy.utils.register_class(XYZObjectPanel)


def unregister():
    bpy.utils.unregister_class(XYZObjectPanel)
    bpy.utils.unregister_class(XYZObjectSettings)


if __name__ == "__main__":
    register()
