#pragma once

void MovimentarDinossauros()
{
	for (int i = 0; i < QuantidadeDinossauros; i++)
	{
		if (Dinossauros[i].morto == true)  /// MUERTO
		{
			Dinossauros[i].X -= 200; // Sai da tela
		}
		else
		{
			if (Dinossauros[i].caindo || Dinossauros[i].abaixado)
			{
				Dinossauros[i].Fitness = Dinossauros[i].Fitness + 2.0;
			}
			else
			{
				Dinossauros[i].Fitness = Dinossauros[i].Fitness + 1.0;
			}
		}
	}
}