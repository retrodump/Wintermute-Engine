*grp

*inc inc_object_attr.inc


*atr Movable
*des Specifies whether the object can be moved by its sprites (in case the sprite frames have a "move" property set).

*atr Scalable
*des Specifies whether the object is affected by scene's scaling levels.

*atr Rotatable
*des Specifies whether the object is affected by scene's rotation levels.

*atr Colorable
*des Specifies whether the object is affected by scene's color regions.


*atr AlphaColor
*des Specifies an RGBA color of this object (overrides scene coloring); set to 0 to reset default behavior.

*atr BlendMode
*des Specifies how the object's image is blended with the background colors; 0...normal mode, 1...additive blending, 2...subtractive blending

*atr Scale
*des Specifies a scale of this object (overrides scene scaling); set to null to reset default behavior.

*atr ScaleX
*des Specifies horizontal scale of this object; set to null to reset default behavior.

*atr ScaleY
*des Specifies vertical scale of this object; set to null to reset default behavior.

*atr RelativeScale
*des Specifies a scale amount to be added to the default scene scale.

*atr Rotate
*des Specifies rotation of this object (in degrees); overrides scene rotation; set to null to reset default behavior; not available in compatibility mode

*atr RelativeRotate
*des Specifies a rotation amount to be added to the default scene rotation.
