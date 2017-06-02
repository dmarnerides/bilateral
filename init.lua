local bilateral = require('bilateral._env')

local type2tensor = { ["torch.FloatTensor"] = torch.FloatTensor,
                        ["torch.DoubleTensor"] = torch.DoubleTensor }

local function filter(img,edge,sigma_s,sigma_r)
    require 'libfilter'
    edge = edge or img
    local w,h = img:size(2), img:size(1)
    sigma_s = sigma_s or math.min(w,h)/60
    sigma_r = sigma_r or (img:max() - img:min())*0.1
    return type2tensor[img:type()].libfilter.lbf(img,edge,w,h,sigma_s,sigma_r)
end
rawset(bilateral, 'filter', filter)

return bilateral