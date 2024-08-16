function stringToNumber(value) {
    return Number.fromLocaleString(Qt.locale(), value)
}

function numberToString(value, precision) {
    if (value === undefined || value === "")
        return undefined
    if (precision === undefined)
        return value.toLocaleString(Qt.locale(), 'f', 2)
    else
        return value.toLocaleString(Qt.locale(), 'f', precision)
}