#!/usr/bin/env python
import os, string, sys, pcpp, io

shaders_dir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'Engine',
                                            'Shaders', 'OpenGL'))
output_dir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'Engine', 'Sources',
                                           'Renderer', 'OpenGL', 'Shaders'))

file_template = string.Template("""
//
//  ${Name}Shader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    ${vars}
}

""")

shader_var_template = string.Template("""
    ${docstr}
    public static var ${name}${Type}ShaderSource: String { return \"""
${source}
\""" }
""")

shader_program_template = string.Template("""
    /// The ${sName} shader program
    public static var ${name}Shader: ShaderProgram? = OpenGLShaderProgram(
        vertexShaderSource:     OpenGLShaderProgram.${vs_name}, 
        fragmentShaderSource:   OpenGLShaderProgram.${fs_name}
    )""")

shaders = []
for root, dirs, files in os.walk(shaders_dir):
    for filename in files:
        if filename.endswith(".vs"):
            shader_name = str(filename[:-3])
            shaders.append(shader_name)


def lowerCaseName(name):
    split_name = list(name)
    for c in range(len(split_name)):
        if split_name[c].isupper() and c < (len(split_name) - 1) and split_name[c + 1].isupper() or c == 0 or c == (
                    len(split_name) - 1):
            split_name[c] = split_name[c].lower()
            c += 1
    return ''.join(split_name)


def spacedName(name):
    split_name = list(name)
    for c in range(len(split_name)):
        if split_name[c].isupper() and c != 0 and c < (len(split_name) - 1) and not split_name[c + 1].isupper() and \
                        split_name[c - 1] != ' ':
            split_name.insert(c, ' ')
            c += 1

    return ''.join(split_name)


def generateShaderVar(source, type, **kwargs):
    kwargs.update({
        'type': type,
        'Type': type[0].upper() + type[1:],
        'source': source
    })

    docstr = findDocumentationString(source)
    if docstr == None:
        docstr = string.Template('/// The ${sName} ${type} shader source').substitute(**kwargs)
    kwargs['docstr'] = docstr

    return kwargs['name'] + kwargs['Type'] + 'ShaderSource', shader_var_template.substitute(**kwargs)


def findDocumentationString(source):
    lines = source.split('\n')

    docstr = []

    index = 0
    for line in lines:
        index += 1
        if line.startswith('#version'):
            continue
        if line == '':
            continue
        if line.startswith('///'):
            docstr += [line]
            continue
        if index >= 8:
            break

    if len(docstr) == 0:
        return None
    else:
        return "\n".join(docstr)


def preprocess(input):
    preprocessor = GLSLPreprocessor()
    preprocessor.add_path(shaders_dir)
    preprocessor.add_path(os.path.join(shaders_dir, 'include'))
    preprocessor.line_directive = None

    preprocessor.parse(input)

    out = io.BytesIO()
    preprocessor.write(out)
    return out.getvalue()


def generateSwiftShader(shader):
    kwargs = {
        'name': lowerCaseName(shader),
        'Name': shader,
        'sName': spacedName(shader)
    }

    fs = vs = gs = None
    fs_name = vs_name = gs_name = None
    vars = []
    with open(os.path.join(shaders_dir, shader + '.vs'), 'r') as f:
        vs = preprocess(f.read())
        vs_name, decl = generateShaderVar(vs, type='vertex', **kwargs)
        vars += [decl]
    with open(os.path.join(shaders_dir, shader + '.fs'), 'r') as f:
        fs = preprocess(f.read())
        fs_name, decl = generateShaderVar(fs, type='fragment', **kwargs)
        vars += [decl]

    gs_path = os.path.join(shaders_dir, shader + '.gs')
    if os.path.exists(gs_path):
        gs = preprocess(f.read())
        gs_name, decl = generateShaderVar(gs, type='geometry', **kwargs)
        vars += [decl]

    kwargs['vars'] = "\n".join(vars)
    kwargs['vs_name'] = vs_name
    kwargs['fs_name'] = fs_name
    kwargs['gs_name'] = gs_name

    code = file_template.substitute(**kwargs)
    with open(os.path.join(output_dir, 'OpenGLShader+' + shader + '.swift'), 'w') as f:
        f.write(code)


class GLSLPreprocessor(pcpp.Preprocessor):
    def on_comment(self, tok):
        return


generateSwiftShader(sys.argv[1])
