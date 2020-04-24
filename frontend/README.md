#Frontend Design for the Application 

##Recent & Important changes:

- switched from gridstack to gridster js<br>
    Gridstack won't allow the serialization of HTML elements<br>
    Gridster does allow that, and made a custom function to do so, as<br
    well as import the data and load it in the form of:
```            
            var serialization = [{
                html: "<div> some html </div>",
                col: 1,
                row: 1,
                size_x: 2,
                size_y: 2
                                }]
```
- added the loadFromSerialization() & serializeGridster() functions mentioned above

- used libraries so far:
    `gridster`
    `jquery`
    `bootstrap`
    `simplified-bootstrap from w3`
