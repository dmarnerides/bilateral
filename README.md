## Bilateral Filter for Torch

A Lua Torch wrapper for the fast bilateral filter by [Sylvain Paris and 
Frédo Durand](http://people.csail.mit.edu/sparis/bf/).

Can also be used for cross/joint bilateral filtering.


## Installation

Make sure you have [Torch](http://torch.ch/) installed.

To install use:
```bash
git clone https://github.com/dmarnerides/bilateral.git
cd bilateral
./install.sh
```

## Note:

* Only accepts two dimensional tensors, i.e. only luminance channels.
* Luminance needs to be [0,1]
* Currently supports:
    * FloatTensor
    * DoubleTensor

## Use

`filter(img,edge,sigma_s,sigma_r)`

Example:
```lua

local bilateral = require('bilateral')

-- Assuming a luminance channel lum, using default values
local filteredLum = bilateral.filter(lum)

-- Cross bilateral filter of lum given edgeLum, with given parameters
local filteredLum = bilateral.filter(lum,edgeLum,sigma_s,sigma_r)

```

To filter an rgb image, linearize it, compute the luminance, filter and then
replace the luminance

Example 2:
```lua
local bilateral = require('bilateral')
local image = require('image')

local img = image.load('dragon.jpg',3,'float') -- load as float [0,1]

-- Linearize (remove gamma, usually gamma = 2.2)
img:pow(2.2)

-- Compute luminance
local lum = img[1]:clone()
lum:mul(0.2126):add(torch.mul(img[2],0.7152)):add(torch.mul(img[3],0.0722))

-- Filter
local filteredLum = bilateral.filter(lum)

-- Replace luminance
local filtered = img:clone()
for i=1,3 do filtered[i]:cmul(filteredLum):cdiv(lum) end
-- Replace NaNs from division with 0
filtered[filtered:ne(filtered)] = 0 

-- Add gamma and save
filtered:pow(1/2.2)
image.save('dragonFiltered.jpg',filtered)

```