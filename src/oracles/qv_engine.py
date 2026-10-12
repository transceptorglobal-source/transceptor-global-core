import sys

def calculate_quadratic_cost(votes):
    return votes ** 2

class AgoraQuadraticVotingEngine:
    def __init__(self, token_balance):
        self.token_balance = token_balance

    def cast_votes(self, user_did, desired_votes):
        cost = calculate_quadratic_cost(desired_votes)
        print(f"\n[2026-09-18T17:59:32+03:00] [INFO] Голосование DID: {user_did}")
        print(f"[INFO] Запрошено голосов: {desired_votes} | Необходимый баланс T_net: {cost}")
        
        if cost > self.token_balance:
            print(f"[WARNING] Отказ! Недостаточно T_net (Баланс: {self.token_balance}, Требуется: {cost})")
            return False
        
        self.token_balance -= cost
        print(f"[TEE_OK] Голоса учтены! Остаток баланса T_net: {self.token_balance}")
        return True

if __name__ == "__main__":
    print("[2026-09-18T17:59:32+03:00] [INFO] Тестирование модуля Квадратичного Голосования Agora QV...")
    qv = AgoraQuadraticVotingEngine(token_balance=100)
    
    # Попытка 1: 5 голосов = 25 T_net
    qv.cast_votes("did:agora:user:0x8888", 5)
    
    # Попытка 2: 10 голосов = 100 T_net (должно отклонить из-за остатка 75)
    qv.cast_votes("did:agora:user:0x8888", 10)
