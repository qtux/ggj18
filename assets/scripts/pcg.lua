width = 200
height = 20
tilesize = 64

math.randomseed(os.time())

file = io.open("assets/levels/level.tmx", "w+")
--file = io.open("../levels/level.tmx", "w+")
io.output(file)
io.write('<?xml version="1.0" encoding="UTF-8"?>\n')
io.write('<map version="1.0" tiledversion="1.1.1" orientation="orthogonal" renderorder="right-down" width="' .. width .. '" height="' .. height .. '" tilewidth="' .. tilesize .. '" tileheight="' .. tilesize .. '" infinite="0" nextobjectid="13">\n')
io.write(' <tileset firstgid="1" source="level.tsx"/>\n')

-- Background Layer
io.write(' <layer name="Background" width="' .. width .. '" height="' .. height .. '">\n')
io.write('  <data encoding="csv">\n')
for i=0,height-1 do
	line = ""
	for j=0,width-2 do
		line = line .. "34,"
	end
	line = line .. "34"
	if i < height-1 then
		line = line .. ","
	end
	io.write(line .. "\n")
end
io.write('</data>\n')
io.write(' </layer>\n')
io.flush(file)

-- Level Layer
io.write(' <layer name="Kachelebene 1" width="' .. width .. '" height="' .. height .. '">\n')
io.write('  <data encoding="csv">\n')
function imitateLowProbability()
	return math.random()^10
end

function imitateHighProbability()
	return math.random()^-3
end

function probability(threshold)
	if math.random() > threshold then
		if threshold < 0.12 or imitateLowProbability() > threshold then
			return 1
		else
			return 0
		end
	else
		return 0
	end
end

function filter(heightIndex, widthIndex)
	-- keep a free start
	if widthIndex < 10 and (heightIndex % 10) < 9 then
		return 0
	elseif widthIndex < 10 and (heightIndex % 10) == 9 then
		return 1
	end
	-- get a probability for a tile
	return probability(1/(heightIndex % 10))
end

local levelArray = {}
for i=0,height-1 do
	levelArray[i] = {}
	for j=0,width-1 do
		levelArray[i][j] = filter(i, j)
	end
end

for i=0,height-1 do
	line = ""
	for j=0,width-2 do
		line = line .. levelArray[i][j] .. ","
	end
	line = line .. levelArray[i][width-1]
	if i < height-1 then
		line = line .. ","
	end
	io.write(line .. "\n")
end
io.write('</data>\n')
io.write(' </layer>\n')
io.flush(file)

-- Collisions
io.write(' <objectgroup name="Collision">\n')
left = -1
for i=0,height-1 do
	for j=0,width-1 do
		if levelArray[i][j] == 1 and left == -1 then
			left = j
		end
		if levelArray[i][j] == 0 and not (left == -1) then
			io.write('  <object id="' .. i+j .. '" x="' .. left*tilesize .. '" y="' .. i*tilesize .. '" width="' .. (j-left)*tilesize .. '" height="' .. tilesize .. '"/>\n')
			left = -1
		end
	end
	if not (left == -1) then
		io.write('  <object id="' .. i+width .. '" x="' .. left*tilesize .. '" y="' .. i*tilesize .. '" width="' .. (width-left)*tilesize .. '" height="' .. tilesize .. '"/>\n')
		left = -1
	end
end
io.write(' </objectgroup>\n')
io.write('</map>')
