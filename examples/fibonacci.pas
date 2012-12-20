program fibonacci;

function inc(n : integer) : void
begin
    n := n + 1
end;

{ very slow! }
function fib(n : integer) : integer
begin
    if n = 0 then begin
        0
    end
    else
        if n = 1 then begin
            1
        end
        else fib(n - 2) + fib(n - 1)
end;

var x : integer;
begin
    x := 0;
    while x < 31 do begin
        write(x);
        write(fib(x));
        inc(x)
    end
end