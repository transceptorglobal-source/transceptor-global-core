import sys

def calculate_quadratic_cost(votes):
    return votes ** 2

class PartyPrimarySystem:
    def __init__(self):
        self.MIN_COMPETENCE_INDEX = 1.50  # Минимальный порог компетентности для кандидата

    def evaluate_candidate(self, candidate_did, competence_index):
        print(f"\n[2026-09-18T19:26:19+03:00] [PARTY_CORE] Оценка кандидата: {candidate_did}")
        print(f"[PARTY_CORE] Текущий Индекс Компетентности (I_cp): {competence_index}")
        
        if competence_index < self.MIN_COMPETENCE_INDEX:
            print(f"[PARTY_REJECT] Кандидат отклонен: I_cp ниже допустимого порога ({self.MIN_COMPETENCE_INDEX})")
            return False
        
        print("[PARTY_OK] Кандидат верифицирован и допущен к партийному праймериз!")
        return True

    def cast_party_votes(self, member_did, candidate_did, votes, token_balance):
        cost = calculate_quadratic_cost(votes)
        print(f"\n[PARTY_VOTE] Партиец {member_did} голосует за {candidate_did}")
        print(f"[PARTY_VOTE] Запрошено голосов: {votes} | Стоимость в баллах влиятельности: {cost}")
        
        if cost > token_balance:
            print(f"[PARTY_WARN] Голосование отклонено: недостаточно баллов влиятельности (Баланс: {token_balance}, Требуется: {cost})")
            return False, token_balance
        
        new_balance = token_balance - cost
        print(f"[PARTY_OK] Голоса учтены! Остаток баллов партийца: {new_balance}")
        return True, new_balance

if __name__ == "__main__":
    print("[2026-09-18T19:26:19+03:00] [INFO] Запуск симулятора внутрипартийного модуля Transceptor Agora...")
    party = PartyPrimarySystem()

    # 1. Оценка кандидатов в списки
    party.evaluate_candidate("did:agora:candidate:0x1111", 1.20)  # Отклонение
    is_valid = party.evaluate_candidate("did:agora:candidate:0x2222", 2.10)  # Одобрение

    # 2. Проведение голосования партийцем
    if is_valid:
        party.cast_party_votes("did:agora:member:0x9999", "did:agora:candidate:0x2222", 4, 20)
