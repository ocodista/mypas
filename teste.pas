program Teste;

    { Declarative scope }
	var i, j: integer;
	    x, y: real;
	    t, w: double;

	function fact(n: integer): integer;
		{ Declarative scope }
		var n: integer;

		{ imperative scope }
		begin
			if n > 2 then
				n := n * fact(n - 1)
			else
				n := n;
			return n {isso n existe em Pascal de verdade}
		end;

    { end declarative scope }

	{ imperative scope }
	begin

		t := w * fact (j)
	end.

	O resto é comentário

