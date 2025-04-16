import tkinter as tk
from tkinter import ttk, messagebox

class FloydApp:
    def __init__(self, root):
        """
        Ініціалізація головного вікна програми.
        Тут створюються всі основні елементи інтерфейсу.
        """
        self.root = root
        self.root.title("Алгоритм Флойда")
        self.root.geometry("820x620")  # Збільшений розмір вікна для кращого відображення
        
        # ===== Вхідні дані =====
        self.input_frame = ttk.LabelFrame(root, text="Матриця ваг графа", padding=10)
        self.input_frame.pack(pady=10, padx=10, fill="x")
        
        # --- Контроль розміру матриці ---
        size_control = ttk.Frame(self.input_frame)
        size_control.pack(fill="x")
        ttk.Label(size_control, text="Розмір матриці:").pack(side="left")
        
        # Вибір розміру матриці (від 2 до 8)
        self.matrix_size = tk.IntVar(value=4)
        ttk.Spinbox(size_control, from_=2, to=8, textvariable=self.matrix_size, 
                   command=self.update_matrix_input).pack(side="left", padx=5)
        
        # --- Таблиця для введення матриці ---
        self.matrix_table = ttk.Frame(self.input_frame)
        self.matrix_table.pack(fill="x", pady=5)
        self.matrix_entries = []  # Тут зберігатимемо всі поля вводу
        self.update_matrix_input()  # Ініціалізуємо таблицю
        
        # ===== Кнопки управління =====
        self.control_frame = ttk.Frame(root)
        self.control_frame.pack(pady=10)
        
        # Кнопка для обчислення результатів
        ttk.Button(self.control_frame, text="Обчислити", command=self.calculate).pack(side="left", padx=5)
        # Кнопка для очищення даних
        ttk.Button(self.control_frame, text="Очистити", command=self.clear).pack(side="left", padx=5)
        
        # ===== Вікно результатів =====
        self.result_frame = ttk.LabelFrame(root, text="Результати", padding=10)
        self.result_frame.pack(pady=10, padx=10, fill="both", expand=True)
        
        # Текстове поле для виведення результатів
        self.result_text = tk.Text(self.result_frame, height=12, font=('Courier New', 10))
        # Додаємо прокрутку
        scrollbar = ttk.Scrollbar(self.result_frame, command=self.result_text.yview)
        scrollbar.pack(side="right", fill="y")
        self.result_text.pack(fill="both", expand=True)
        self.result_text.configure(yscrollcommand=scrollbar.set)
        
    def update_matrix_input(self):
        """
        Оновлює таблицю для введення матриці відповідно до обраного розміру.
        Видаляє старі поля вводу та створює нові.
        """
        # Очищаємо старі поля
        for widget in self.matrix_table.winfo_children():
            widget.destroy()
        self.matrix_entries = []  # Очищаємо список полів
        
        size = self.matrix_size.get()  # Отримуємо поточний розмір
        
        # --- Створюємо заголовки стовпців ---
        for j in range(size):
            ttk.Label(self.matrix_table, text=f"До {j+1}", width=6).grid(row=0, column=j+1)
        
        # --- Створюємо поля вводу ---
        for i in range(size):
            # Додаємо мітки рядків
            ttk.Label(self.matrix_table, text=f"Від {i+1}").grid(row=i+1, column=0)
            
            row_entries = []  # Тут зберігатимемо поля поточного рядка
            for j in range(size):
                entry = ttk.Entry(self.matrix_table, width=6)
                
                # Заповнюємо діагональ нулями, інші поля - "∞"
                if i == j:
                    entry.insert(0, "0")
                else:
                    entry.insert(0, "∞")
                
                entry.grid(row=i+1, column=j+1, padx=2, pady=2)
                row_entries.append(entry)
            
            self.matrix_entries.append(row_entries)  # Додаємо рядок до загального списку
    
    def get_matrix(self):
        """
        Отримує матрицю з полів вводу.
        Перетворює текст у числові значення ("∞" стає нескінченністю).
        Повертає двовимірний список (матрицю).
        """
        size = self.matrix_size.get()
        matrix = []
        infinity = float('inf')  # Представлення нескінченності
        
        for i in range(size):
            row = []
            for j in range(size):
                value = self.matrix_entries[i][j].get().strip()  # Отримуємо значення
                
                # Обробляємо спеціальні значення
                if value.lower() == "∞" or value == "":
                    row.append(infinity)
                else:
                    try:
                        row.append(int(value))  # Спробуємо перетворити в число
                    except ValueError:
                        row.append(infinity)  # Якщо не вийшло - ставимо ∞
            matrix.append(row)
        
        return matrix
    
    def floyd_algorithm(self, graph):
        """
        Реалізація алгоритму Флойда для пошуку найкоротших шляхів.
        Приймає матрицю суміжності графа.
        Повертає матрицю найкоротших відстаней або None, якщо є від'ємний цикл.
        """
        n = len(graph)
        dist = [row[:] for row in graph]  # Копіюємо матрицю
        
        # Основний алгоритм
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    # Оновлюємо відстань, якщо знайдено коротший шлях через k
                    if dist[i][k] + dist[k][j] < dist[i][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]
        
        # Перевірка на наявність від'ємних циклів
        for i in range(n):
            if dist[i][i] < 0:  # Якщо діагональний елемент від'ємний
                return None  # Граф містить від'ємний цикл
        
        return dist
    
    def calculate(self):
        """
        Обробник натискання кнопки "Обчислити".
        Отримує матрицю, обчислює результати та виводить їх.
        """
        try:
            matrix = self.get_matrix()  # Отримуємо матрицю з полів вводу
            result = self.floyd_algorithm(matrix)  # Обчислюємо результати
            
            self.result_text.delete(1.0, tk.END)  # Очищаємо попередні результати
            
            if result is None:
                self.result_text.insert(tk.END, "Граф містить від'ємний цикл!")
                return
            
            # --- Форматуємо вивід результатів ---
            self.result_text.insert(tk.END, "Матриця найкоротших відстаней:\n\n")
            size = len(result)
            
            # Заголовки стовпців
            header = "        " + "".join([f"{'До ' + str(j+1):<8}" for j in range(size)])
            self.result_text.insert(tk.END, header + "\n")
            
            # Дані матриці
            for i in range(size):
                row_str = f"Від {i+1}:  "
                for j in range(size):
                    val = result[i][j]
                    if val == float('inf'):
                        row_str += "∞".ljust(8)  # Вирівнюємо ∞
                    else:
                        row_str += f"{val}".ljust(8)  # Вирівнюємо числа
                self.result_text.insert(tk.END, row_str + "\n")
                
        except Exception as e:
            messagebox.showerror("Помилка", f"Сталася помилка: {str(e)}")
    
    def clear(self):
        """
        Обробник натискання кнопки "Очистити".
        Очищає результати та оновлює таблицю вводу.
        """
        self.result_text.delete(1.0, tk.END)  # Очищаємо результати
        self.update_matrix_input()  # Оновлюємо таблицю вводу

if __name__ == "__main__":
    root = tk.Tk()  # Створюємо головне вікно
    app = FloydApp(root)  # Створюємо екземпляр нашої програми
    root.mainloop()  # Запускаємо головний цикл програми