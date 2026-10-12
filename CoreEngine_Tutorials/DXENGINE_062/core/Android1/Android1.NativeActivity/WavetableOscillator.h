#pragma once

namespace wavetablesynthesizer {

	class A4Oscillator
	{
	public:
		// Construtor que recebe a taxa de amostragem
		A4Oscillator(float sampleRate);

		// Funções membro da classe
		float getSample();
		void onPlaybackStopped();

	private:
		// Variáveis privadas obrigatórias (usadas no teu .cpp)
		float _phase = 0.0f;
		float _phaseIncrement = 0.0f;
	};

} // namespace wavetablesynthesizer
