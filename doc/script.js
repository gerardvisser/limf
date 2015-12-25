var displayed = null;
var linkElem = null;

function display (clickedLink, id) {
    var body = document.getElementById ("body");
    var cache = document.getElementById ("cache");
    if (displayed != null) {
        body.removeChild (displayed);
        cache.appendChild (displayed);
        displayed = null;
    }
    var toBeDisplayed = document.getElementById (id);
    if (toBeDisplayed != null) {
        displayed = cache.removeChild (toBeDisplayed);
        body.appendChild (displayed);
    }
    if (!(linkElem == null | linkElem == clickedLink)) {
        linkElem.style.backgroundColor = "#FFFFFF";
    }
    linkElem = clickedLink;
    linkElem.style.backgroundColor = "#D0D0FF";
}

function init () {
    var container = document.getElementById ("container");
    var containerHeight = container.offsetHeight;
    var containerWidth = container.offsetWidth - 2 * container.offsetLeft;
    container.style.width = containerWidth + "px";
    var head = document.getElementById ("head");
    var index = document.getElementById ("index");
    var body = document.getElementById ("body");
    var h = containerHeight - head.offsetHeight - 3;
    index.style.height = h + "px";
    body.style.height = h + "px";
    body.style.width = containerWidth - body.offsetLeft - 4 + "px";
}
