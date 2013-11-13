*grp Item functions

*met TakeItem(ItemName, InsertAfter)
*des Adds an item into the inventory.
*par ItemName A name of the item to be added to object's inventory.
*par InsertAfter A name of the item which the new item should be inserted after (optional).

*met DropItem(ItemName)
*des Removes an item from the inventory.
*par ItemName A name of the item to be removed from object's inventory.

*met GetItem(ItemIndex)
*mt2 GetItem(ItemName)
*des Queries an inventory item.
*par ItemIndex An index of a <b>taken item</b> to be queried.
*par ItemName A name of an inventory item to be queried.
*ret A reference to the item object of a given name or at a given index in the inventory.


*met HasItem(ItemName)
*mt2 HasItem(ItemObject)
*des Queries whether a given item is in inventory of any of this object.
*par ItemName A name of the item to be queried.
*par ItemObject Reference to the item object to be queried.
*ret If the specified item is in the inventory of this object, the return value is <b>true</b>.
*rem Note that this method only searches the current object, use the <b>Game.IsItemTaken()</b> method if you want to search all game objects.
