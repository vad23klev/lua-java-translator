--- A simple Dog class.
-- @classmod Dog
Dog = {}


--- Makes a dog with a specified voice.
-- @param sound Dog's voice
-- @return Constructed dog
function Dog:new(sound)
    local newObj = {
        voice = sound
    }
    self.__index = self
    return setmetatable(newObj, self)
end


--- Makes sound
function Dog:makeSound()
    if (self.voice == nil) then
        error('The dog is deaf')
    end
    print(self.voice)
end


--- Changes dog's voice.
-- @param sound A new voice
function Dog:setVoice(sound)
    if (sound == nil) then
        error('The dog cannot be stunned.')
    end
    self.voice = sound
end


--- Compares self dog to other.
-- @param other Other dog
-- @return Equality
function Dog:__eq(other)
    return self.voice == other.voice
end


--- Creates a new dog with heads spliced in one body
-- @param other Other dog
-- @return Dog with few heads
function Dog:__concat(other)
    return Dog:new(self.voice .. ' ' .. other.voice)
end


-- ------------- --
--  Main chunk.  --
-- ------------- --
dog1 = Dog:new()
dog2 = Dog:new('woof')
dog3 = Dog:new('woof')

print(dog1 == dog2 and 'dog1 is equal to dog2' or 'dog1 is not equal to dog2')
print(dog2 == dog3 and 'dog2 is equal to dog3' or 'dog2 is not equal to dog3')

print("Let's dog2 make sound")
dog2:makeSound()

print("Let's change dog2's voice")
dog2:setVoice('woof motherfucker')
dog2:makeSound()

print("And dog3's voice...")
dog3:makeSound()

print("Let's make a strange creature with heads of dog3 and dog2")
dog4 = dog3 .. dog2
dog4:makeSound()
