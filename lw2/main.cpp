//
// Created by Вадим Патрушев on 18.10.2026.
//

// // Прямоугольник пикселей [x0, x1) × [y0, y1)
// struct Rect { int x0, y0, x1, y1; };
//
// // Размывает пиксели rc: читает из src, пишет в dst
// void BlurRect(const Image& src, Image& dst, Rect rc, int radius);
//
// // Для каждого потока — список его прямоугольников (зависит от варианта)
// std::vector<std::vector<Rect>> MakeWork(int width, int height, int threadCount);
//
// // ...в main, после чтения файла
// Image dst = /* пустое изображение того же размера */;
// auto work = MakeWork(src.width, src.height, threadCount);
//
// const auto start = std::chrono::steady_clock::now();
// {
// 	std::vector<std::jthread> threads;
// 	for (int k = 0; k < threadCount; ++k)
// 	{
// 		threads.emplace_back(Worker, std::cref(src), std::ref(dst),
// 							 std::move(work[k]), radius);
// 	}
// } // здесь все потоки завершены: деструкторы jthread сделали join
// const auto finish = std::chrono::steady_clock::now();
