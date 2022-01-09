import csv
import os

blocks = []

mesh_export = open('meshexport.csv','r')
readexport = csv.DictReader(mesh_export)

block_defs = open('block_definitions.csv','r')
readdefs = csv.DictReader(block_defs)

for row in readdefs:
    blocks.append(row)


for row in readexport:
    block_name = row['Name']
    found = False
    for entry in blocks:
        if entry['Name'] == block_name:
            found = True
            break
    
    if not found:
        blocks.append({"Name": block_name})


write_block_defs = open('new_block_definitions.csv','w',newline='', encoding='utf-8')
colnames = blocks[0].keys()
writedefs = csv.DictWriter(write_block_defs,colnames)
writedefs.writeheader()
writedefs.writerows(blocks)

mesh_export.close()
block_defs.close()
write_block_defs.close()

os.remove('old_block_definitions.csv')
os.rename('block_definitions.csv','old_block_definitions.csv')
os.rename('new_block_definitions.csv','block_definitions.csv')

blockenum = "enum block_type { " #air, dirt, grass_drit, stone, grass_ramp};"
for block in blocks:
    enum = block['Name'].replace(' ','_').lower()
    blockenum = blockenum + (enum +', ')
blockenum = blockenum + '};\n'


block_str = """static const std::vector<std::string> block_str = { """ #Air", "Dirt", "Grassy Dirt", "Stone", "Grass Ramp"};"""
for block in blocks:
    b_str = block["Name"]
    block_str = block_str + ('"'+b_str+'", ')
block_str = block_str + '};\n'


blockenumvect = """static const std::vector<block_type> block_emums = {""" #block_type::air,block_type::dirt,block_type::grass_drit,block_type::stone, block_type ::grass_ramp};"""
for block in blocks:
    b_vect = block["Name"].replace(' ','_').lower()
    blockenumvect = blockenumvect + ('block_type::'+b_vect+', ')
blockenumvect = blockenumvect + '};'

header = open('block_ids.h','w')
header.writelines(['#pragma once\n',blockenum,block_str,blockenumvect])
header.close()