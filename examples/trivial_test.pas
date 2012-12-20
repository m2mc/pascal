program hello_world;

function foo : boolean
begin
    bar(2, 1) 
end;

function bar(a, b : integer) : integer
var x : integer;
begin
    x := 100000;
    while x > 0 do x := x - 1 - 2
end;

function buzz(a : integer) : void
begin

end;

function inc(a : integer) : void
begin
    a := a + 1
end;

var i, j : integer;
var a, b : boolean;

begin
    i := 5;
    inc(i);
    inc(5);
    inc(5);
    inc(i)
end